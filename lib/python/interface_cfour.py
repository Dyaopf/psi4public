#
#@BEGIN LICENSE
#
# PSI4: an ab initio quantum chemistry software package
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#@END LICENSE
#

"""Module with functions for Psi4/Cfour interface. Portions that require
calls to Boost Python psi4 module are here, otherwise in qcdb module.
Also calls to qcdb module are here and not elsewhere in driver.
Organizationally, this module isolates qcdb code from psi4 code.

"""
from __future__ import print_function
import shutil
import os
import subprocess
import re
import inspect
import psi4
import p4const
import p4util
import qcdb
from p4regex import *
#from extend_Molecule import *
from molutil import *
from functional import *
# never import driver, wrappers, or aliases into this file


def run_cfour(name, **kwargs):
    """Function that prepares environment and input files
    for a calculation calling Stanton and Gauss's CFOUR code.
    Also processes results back into Psi4 format.

    This function is not called directly but is instead called by
    :py:func:`~driver.energy` or :py:func:`~driver.optimize` when a Cfour
    method is requested (through *name* argument). In order to function
    correctly, the Cfour executable ``xcfour`` must be present in
    :envvar:`PATH` or :envvar:`PSIPATH`.

    :type name: string
    :param name: ``'c4-scf'`` || ``'c4-ccsd(t)'`` || ``'cfour'`` || etc.

	    First argument, usually unlabeled. Indicates the computational
	    method to be applied to the system.

    :type keep: :ref:`boolean <op_py_boolean>`
    :param keep: ``'on'`` || |dl| ``'off'`` |dr|

	    Indicates whether to delete the Cfour scratch directory upon
	    completion of the Cfour job.

    :type path: string
    :param path:

	    Indicates path to Cfour scratch directory. Otherwise, the
	    default is a subdirectory within the Psi4 scratch directory.

    """
    lowername = name.lower()

    # Because this fn is called for energy('cfour'), opt('cfour'), etc.,
    # need this to figure out who called
    dertype_dict = {'energy': 0, 'gradient': 1, 'hessian': 2}
    dertype = dertype_dict[inspect.stack()[1][3]]
    print('I am %s called by %s called by %s.\n' % 
        (inspect.stack()[0][3], inspect.stack()[1][3], inspect.stack()[2][3]))

    # Save submission directory
    current_directory = os.getcwd()

    # Move into job scratch directory
    psioh = psi4.IOManager.shared_object()
    psio = psi4.IO.shared_object()
    os.chdir(psioh.get_default_path())

    # Construct and move into cfour subdirectory of job scratch directory
    cfour_tmpdir = kwargs['path'] if 'path' in kwargs else \
        'psi.' + str(os.getpid()) + '.' + psio.get_default_namespace() + \
        '.cfour.' + str(random.randint(0, 99999))
    if not os.path.exists(cfour_tmpdir):
        os.mkdir(cfour_tmpdir)
    os.chdir(cfour_tmpdir)

    # Find environment by merging PSIPATH and PATH environment variables
    lenv = os.environ
    lenv['PATH'] = ':'.join([os.path.abspath(x) for x in os.environ.get('PSIPATH', '').split(':')]) + ':' + lenv.get('PATH')

    # Load the GENBAS file
    genbas_path = qcdb.search_file('GENBAS', lenv['PATH'])
    if genbas_path:
        shutil.copy2(genbas_path, psioh.get_default_path() + cfour_tmpdir)
        psi4.print_out("\n  GENBAS loaded from %s\n" % (genbas_path))
        psi4.print_out("  CFOUR to be run from %s\n" % (psioh.get_default_path() + cfour_tmpdir))
    else:
        message = """
  GENBAS file for CFOUR interface not found. Either:
  [1] Supply a GENBAS by placing it in PATH or PSIPATH
      [1a] Use cfour {} block with molecule and basis directives.
      [1b] Use molecule {} block and CFOUR_BASIS keyword.
  [2] Allow PSI4's internal basis sets to convert to GENBAS
      [2a] Use molecule {} block and BASIS keyword.

"""
        psi4.print_out(message)
        psi4.print_out('  Search path that was tried:\n')
        psi4.print_out(lenv['PATH'].replace(':', ', '))

    # Generate the ZMAT input file in scratch and write to outfile
    with open('ZMAT', 'w') as cfour_infile:
        cfour_infile.write(write_zmat(lowername, dertype))
    psi4.print_out('\n====== Begin ZMAT input for CFOUR ======\n')
    psi4.print_out(open('ZMAT', 'r').read())
    psi4.print_out('======= End ZMAT input for CFOUR =======\n\n')
    #print('\n====== Begin ZMAT input for CFOUR ======\n', open('ZMAT', 'r').read(), \
    #    '======= End ZMAT input for CFOUR =======\n\n')

    # Close psi4 output file and reopen with filehandle
    psi4.close_outfile()
    p4out = open(current_directory + '/' + psi4.outfile_name(), 'a')

    # Handle user's OMP_NUM_THREADS and CFOUR_OMP_NUM_THREADS
    omp_num_threads_found = 'OMP_NUM_THREADS' in os.environ
    if omp_num_threads_found == True:
        omp_num_threads_user = os.environ['OMP_NUM_THREADS']
    if psi4.has_option_changed('CFOUR', 'CFOUR_OMP_NUM_THREADS') == True:
        os.environ['OMP_NUM_THREADS'] = str(psi4.get_option('CFOUR', 'CFOUR_OMP_NUM_THREADS'))

    print("""\n\n<<<<<  RUNNING CFOUR ...  >>>>>\n\n""")
    # Call executable xcfour, directing cfour output to the psi4 output file
    try:
        retcode = subprocess.Popen(['xcfour'], bufsize=0, stdout=subprocess.PIPE, env=lenv)
    except OSError as e:
        sys.stderr.write('Program xcfour not found in path or execution failed: %s\n' % (e.strerror))
        p4out.write('Program xcfour not found in path or execution failed: %s\n' % (e.strerror))
        sys.exit(1)

    c4out = ''
    while True:
        data = retcode.stdout.readline()
        if not data:
            break
        if psi4.outfile_name() == 'stdout':
            sys.stdout.write(data)
        else:
            p4out.write(data)
            p4out.flush()
        c4out += data

    # Restore user's OMP_NUM_THREADS
    if omp_num_threads_found == True:
        if psi4.has_option_changed('CFOUR', 'CFOUR_OMP_NUM_THREADS') == True:
            os.environ['OMP_NUM_THREADS'] = omp_num_threads_user






    c4files = {}
    p4out.write('\n')
    for item in ['GRD', 'FCMFINAL']:
        try:
            with open(psioh.get_default_path() + cfour_tmpdir + '/' + item, 'r') as handle:
                c4files[item] = handle.read()
                p4out.write('  CFOUR scratch file %s has been read\n' % (item))
                p4out.write('%s\n' % c4files[item])
        except IOError:
            pass
    p4out.write('\n')

    molecule = psi4.get_active_molecule()
    if molecule.name() == 'blank_molecule_psi4_yo':
        qcdbmolecule = None
    else:
        molecule.update_geometry()
        #print('ZZZZ')
        #print(molecule.save_string_xyz())
        #print(molecule.create_psi4_string_from_molecule())
        qcdbmolecule = qcdb.Molecule(molecule.create_psi4_string_from_molecule())
        qcdbmolecule.update_geometry()
        #print('zzzz')
        #qcdbmolecule.print_out_in_bohr()






    psivar, c4grad = qcdb.cfour.harvest(qcdbmolecule, c4out, **c4files)




    # Absorb results into psi4 data structures
    for key in psivar.keys():
        psi4.set_variable(key.upper(), float(psivar[key]))

    if c4grad:
        mat = psi4.Matrix(len(c4grad), 3)
        mat.set(c4grad)
        psi4.set_gradient(mat)

        #print '    <<<   [3] C4-GRD-GRAD   >>>'
        #mat.print()

#    exit(1)

    # # Things needed psi4.so module to do
    # collect c4out string
    # read GRD
    # read FCMFINAL
    # see if theres an active molecule
    # 
    # # Things delegatable to qcdb
    # parsing c4out
    # reading GRD and FCMFINAL strings
    # reconciling p4 and c4 molecules (orient)
# reconciling c4out and GRD and FCMFINAL results
# transforming frame of results back to p4
    # 
    # # Things run_cfour needs to have back
    # psivar
# qcdb.Molecule of c4?
# coordinates?
# gradient in p4 frame





#    # Process the cfour output
#    psivar, c4coord, c4grad = qcdb.cfour.cfour_harvest(c4out)
#    for key in psivar.keys():
#        psi4.set_variable(key.upper(), float(psivar[key]))
#
#    # Awful Hack - Go Away TODO
#    if c4grad:
#        molecule = psi4.get_active_molecule()
#        molecule.update_geometry()
#
#        if molecule.name() == 'blank_molecule_psi4_yo':
#            p4grad = c4grad
#            p4coord = c4coord
#        else:
#            qcdbmolecule = qcdb.Molecule(molecule.create_psi4_string_from_molecule())
#            #p4grad = qcdbmolecule.deorient_array_from_cfour(c4coord, c4grad)
#            #p4coord = qcdbmolecule.deorient_array_from_cfour(c4coord, c4coord)
#
#            with open(psioh.get_default_path() + cfour_tmpdir + '/GRD', 'r') as cfour_grdfile:
#                c4outgrd = cfour_grdfile.read()
#            print('GRD\n',c4outgrd)
#            c4coordGRD, c4gradGRD = qcdb.cfour.cfour_harvest_files(qcdbmolecule, grd=c4outgrd)
#
#        p4mat = psi4.Matrix(len(p4grad), 3)
#        p4mat.set(p4grad)
#        psi4.set_gradient(p4mat)

#    print('    <<<  P4 PSIVAR  >>>')
#    for item in psivar:
#        print('       %30s %16.8f' % (item, psivar[item]))
    #print('    <<<  P4 COORD   >>>')
    #for item in p4coord:
    #    print('       %16.8f %16.8f %16.8f' % (item[0], item[1], item[2]))
#    print('    <<<   P4 GRAD   >>>')
#    for item in c4grad:
#        print('       %16.8f %16.8f %16.8f' % (item[0], item[1], item[2]))














    # Clean up cfour scratch directory unless user instructs otherwise
    keep = yes.match(str(kwargs['keep'])) if 'keep' in kwargs else False
    os.chdir('..')
    try:
        if keep or ('path' in kwargs):
            p4out.write('\n  CFOUR scratch files have been kept in %s\n' % (psioh.get_default_path() + cfour_tmpdir))
        else:
            shutil.rmtree(cfour_tmpdir)
    except OSError as e:
        print('Unable to remove CFOUR temporary directory %s' % e, file=sys.stderr)
        exit(1)

    # Return to submission directory and reopen output file
    os.chdir(current_directory)
    psi4.reopen_outfile()

    d2d = ['Energy', 'Gradient', 'Hessian']
    p4util.banner(' Cfour %s %s Results \n' % (name.lower(), d2d[dertype]))
    psi4.print_variables()
    if c4grad:
        psi4.get_gradient().print_out()

    # Quit if Cfour threw error
    if psi4.get_variable('CFOUR ERROR CODE'):
        raise ValidationError("""Cfour exited abnormally.""")


def cfour_list():
    return qcdb.cfour.cfour_list()

def cfour_gradient_list():
    return qcdb.cfour.cfour_gradient_list()

def cfour_psivar_list():
    return qcdb.cfour.cfour_psivar_list()

def write_zmat(name, dertype):
    """

    """
    # Handle memory
    mem = int(0.000001 * psi4.get_memory())
    if mem == 256:
        memcmd, memkw = '', {}
    else:
        memcmd, memkw = qcdb.cfour.cfour_memory(mem)

    # Handle molecule and basis set
    molecule = psi4.get_active_molecule()
    if molecule.name() == 'blank_molecule_psi4_yo':
        molcmd, molkw = '', {}
        bascmd, baskw = '', {}
    else:
        molecule.update_geometry()
        #print(molecule.create_psi4_string_from_molecule())
        qcdbmolecule = qcdb.Molecule(molecule.create_psi4_string_from_molecule())
        qcdbmolecule.tagline = molecule.name()
        molcmd, molkw = qcdbmolecule.format_molecule_for_cfour()

        if psi4.get_global_option('BASIS') == '':
            bascmd, baskw = '', {}
        else:
            user_pg = molecule.schoenflies_symbol()
            molecule.reset_point_group('c1')
            with open('GENBAS', 'w') as cfour_basfile:
                cfour_basfile.write(psi4.BasisSet.construct(psi4.Gaussian94BasisSetParser(), molecule, "BASIS").genbas())
            psi4.print_out('  GENBAS loaded from PSI4 LibMints for basis %s\n' % (psi4.get_global_option('BASIS')))
            molecule.reset_point_group(user_pg)
            molecule.update_geometry()
            bascmd, baskw = qcdbmolecule.format_basis_for_cfour(psi4.MintsHelper().basisset().has_puream())

    # Handle calc type
    clvcmd, clvkw = qcdb.cfour.cfour_calclevel(dertype)
    # consider combining calclevel and method so can set ecc/vcc diff by dertype

    # Handle psi4 keywords implying cfour keyword values (NYI)

    # Handle quantum chemical method
    mtdcmd, mtdkw = qcdb.cfour.cfour_method(name)

    # Handle driver vs input/default keyword reconciliation
    userkw = p4util.prepare_options_for_modules()
    userkw = qcdb.options.reconcile_options(userkw, memkw)
    userkw = qcdb.options.reconcile_options(userkw, molkw)
    userkw = qcdb.options.reconcile_options(userkw, baskw)
    userkw = qcdb.options.reconcile_options(userkw, mtdkw)
    userkw = qcdb.options.reconcile_options(userkw, clvkw)

    # Handle conversion of psi4 keyword structure into cfour format
    optcmd = qcdb.options.prepare_options_for_cfour(userkw)

    # Handle text to be passed untouched to cfour
    litcmd = psi4.get_global_option('LITERAL_CFOUR')

    zmat = memcmd + molcmd + optcmd + mtdcmd + bascmd + litcmd
    return zmat
