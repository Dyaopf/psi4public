#
# @BEGIN LICENSE
#
# Psi4: an open-source quantum chemistry software package
#
# Copyright (c) 2007-2016 The Psi4 Developers.
#
# The copyrights for code used from other parties are included in
# the corresponding files.
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
# @END LICENSE
#

"""Module with functions that call the four main :py:mod:`driver`
functions: :py:mod:`driver.energy`, :py:mod:`driver.optimize`,
:py:mod:`driver.response`, and :py:mod:`driver.frequency`.

"""
from __future__ import absolute_import
import re
import os
import math
import warnings
import pickle
import copy
import collections
import p4const
from driver import *
from procedures.interface_cfour import cfour_psivar_list
# never import aliases into this file


def return_energy_components():
    VARH = {}
    VARH['scf'] = {
                            'scf': 'SCF TOTAL ENERGY'}
    VARH['hf'] = {
                             'hf': 'HF TOTAL ENERGY'}
    VARH['mp2'] = {
                             'hf': 'HF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY'}
    VARH['mp2.5'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY'}
    VARH['mp3'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY'}
    VARH['mp4(sdq)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY',
                       'mp4(sdq)': 'MP4(SDQ) TOTAL ENERGY'}
    VARH['mp4'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY',
                       'mp4(sdq)': 'MP4(SDQ) TOTAL ENERGY',
                            'mp4': 'MP4(SDTQ) TOTAL ENERGY'}
    VARH['omp2'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'omp2': 'OMP2 TOTAL ENERGY'}
    VARH['omp2.5'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                         'omp2.5': 'OMP2.5 TOTAL ENERGY'}
    VARH['omp3'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY',
                           'omp3': 'OMP3 TOTAL ENERGY'}
    VARH['olccd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'olccd': 'OLCCD TOTAL ENERGY'}
    VARH['lccd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'lccd': 'LCCD TOTAL ENERGY'}
    VARH['lccsd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'lccsd': 'LCCSD TOTAL ENERGY'}
    VARH['cepa(0)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                        'cepa(0)': 'CEPA(0) TOTAL ENERGY'}
    VARH['cepa(1)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                        'cepa(1)': 'CEPA(1) TOTAL ENERGY'}
    VARH['cepa(3)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                        'cepa(3)': 'CEPA(3) TOTAL ENERGY'}
    VARH['acpf'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'acpf': 'ACPF TOTAL ENERGY'}
    VARH['aqcc'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'aqcc': 'AQCC TOTAL ENERGY'}
    VARH['qcisd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY',
                       'mp4(sdq)': 'MP4(SDQ) TOTAL ENERGY',
                          'qcisd': 'QCISD TOTAL ENERGY'}
    VARH['cc2'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                            'cc2': 'CC2 TOTAL ENERGY'}
    VARH['ccsd'] = {
                             'hf': 'HF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'ccsd': 'CCSD TOTAL ENERGY'}
    VARH['bccd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'bccd': 'CCSD TOTAL ENERGY'}
    VARH['cc3'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                            'cc3': 'CC3 TOTAL ENERGY'}
    VARH['fno-ccsd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                       'fno-ccsd': 'CCSD TOTAL ENERGY'}
    VARH['fno-ccsd(t)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'ccsd': 'CCSD TOTAL ENERGY',
                    'fno-ccsd(t)': 'CCSD(T) TOTAL ENERGY'}
    VARH['qcisd(t)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                          'mp2.5': 'MP2.5 TOTAL ENERGY',
                            'mp3': 'MP3 TOTAL ENERGY',
                       'mp4(sdq)': 'MP4(SDQ) TOTAL ENERGY',
                          'qcisd': 'QCISD TOTAL ENERGY',
                       'qcisd(t)': 'QCISD(T) TOTAL ENERGY'}
    VARH['ccsd(t)'] = {
                             'hf': 'HF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'ccsd': 'CCSD TOTAL ENERGY',
                        'ccsd(t)': 'CCSD(T) TOTAL ENERGY'}
    VARH['bccd(t)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                           'ccsd': 'CCSD TOTAL ENERGY',
                        'bccd(t)': 'CCSD(T) TOTAL ENERGY'}
    VARH['cisd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                           'cisd': 'CISD TOTAL ENERGY'}
    VARH['cisdt'] = {
                            'scf': 'SCF TOTAL ENERGY',
                          'cisdt': 'CISDT TOTAL ENERGY'}
    VARH['cisdtq'] = {
                            'scf': 'SCF TOTAL ENERGY',
                         'cisdtq': 'CISDTQ TOTAL ENERGY'}
    VARH['fci'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'fci': 'FCI TOTAL ENERGY'}
    VARH['mrccsd'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                         'mrccsd': 'CCSD TOTAL ENERGY'}
    VARH['mrccsd(t)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                         'mrccsd': 'CCSD TOTAL ENERGY',
                      'mrccsd(t)': 'CCSD(T) TOTAL ENERGY'}
    VARH['mrccsdt'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                        'mrccsdt': 'CCSDT TOTAL ENERGY'}
    VARH['mrccsdt(q)'] = {
                            'scf': 'SCF TOTAL ENERGY',
                            'mp2': 'MP2 TOTAL ENERGY',
                        'mrccsdt': 'CCSDT TOTAL ENERGY',
                     'mrccsdt(q)': 'CCSDT(Q) TOTAL ENERGY'}

    for cilevel in range(2, 99):
        VARH['ci%s' % (str(cilevel))] = {
                            'scf': 'SCF TOTAL ENERGY',
          'ci%s' % (str(cilevel)): 'CI TOTAL ENERGY'}

    for mplevel in range(5, 99):
        VARH['mp%s' % (str(mplevel))] = {
                             'hf': 'HF TOTAL ENERGY',
          'mp%s' % (str(mplevel)): 'MP%s TOTAL ENERGY' % (str(mplevel))}
        for mplevel2 in range(2, mplevel):
            VARH['mp%s' % (str(mplevel))]['mp%s' % (str(mplevel2))] = \
                                      'MP%s TOTAL ENERGY' % (str(mplevel2))

    # Integrate CFOUR methods
    VARH.update(cfour_psivar_list())
    return VARH

VARH=return_energy_components()

def _autofragment_convert(p, symbol):
    # Finding radii for auto-fragmenter
    if symbol[p] == 'H':
        d = 1.001
    if symbol[p] == 'He':
        d = 1.012
    if symbol[p] == 'Li':
        d = 0.825
    if symbol[p] == 'Be':
        d = 1.408
    if symbol[p] == 'B':
        d = 1.485
    if symbol[p] == 'C':
        d = 1.452
    if symbol[p] == 'N':
        d = 1.397
    if symbol[p] == 'O':
        d = 1.342
    if symbol[p] == 'F':
        d = 1.287
    if symbol[p] == 'Ne':
        d = 1.243
    if symbol[p] == 'Na':
        d = 1.144
    if symbol[p] == 'Mg':
        d = 1.364
    if symbol[p] == 'Al':
        d = 1.639
    if symbol[p] == 'Si':
        d = 1.716
    if symbol[p] == 'P':
        d = 1.705
    if symbol[p] == 'S':
        d = 1.683
    if symbol[p] == 'Cl':
        d = 1.639
    if symbol[p] == 'Ar':
        d = 1.595

    return d / 1.5


def auto_fragments(**kwargs):
    r"""Detects fragments if the user does not supply them.
    Currently only used for the WebMO implementation of SAPT.

    :returns: :ref:`Molecule<sec:psimod_Molecule>`) |w--w| fragmented molecule.

    :type molecule: :ref:`molecule <op_py_molecule>`
    :param molecule: ``h2o`` || etc.

        The target molecule, if not the last molecule defined.

    :examples:

    >>> # [1] replicates with cbs() the simple model chemistry scf/cc-pVDZ: set basis cc-pVDZ energy('scf')
    >>> molecule mol {\nH 0.0 0.0 0.0\nH 2.0 0.0 0.0\nF 0.0 1.0 0.0\nF 2.0 1.0 0.0\n}
    >>> print mol.nfragments()  # 1
    >>> fragmol = auto_fragments()
    >>> print fragmol.nfragments()  # 2

    """
    # Make sure the molecule the user provided is the active one
    molecule = kwargs.pop('molecule', psi4.get_active_molecule())
    molecule.update_geometry()
    molname = molecule.name()

    geom = molecule.save_string_xyz()

    numatoms = molecule.natom()
    VdW = [1.2, 1.7, 1.5, 1.55, 1.52, 1.9, 1.85, 1.8]

    symbol = list(range(numatoms))
    X = [0.0] * numatoms
    Y = [0.0] * numatoms
    Z = [0.0] * numatoms

    Queue = []
    White = []
    Black = []
    F = geom.split('\n')
    for f in range(numatoms):
        A = F[f + 1].split()
        symbol[f] = A[0]
        X[f] = float(A[1])
        Y[f] = float(A[2])
        Z[f] = float(A[3])
        White.append(f)
    Fragment = [[] for i in range(numatoms)]  # stores fragments

    start = 0  # starts with the first atom in the list
    Queue.append(start)
    White.remove(start)

    frag = 0

    while((len(White) > 0) or (len(Queue) > 0)):  # Iterates to the next fragment
        while(len(Queue) > 0):  # BFS within a fragment
            for u in Queue:  # find all nearest Neighbors
                             #   (still coloured white) to vertex u
                for i in White:
                    Distance = math.sqrt((X[i] - X[u]) * (X[i] - X[u]) +
                                         (Y[i] - Y[u]) * (Y[i] - Y[u]) +
                                         (Z[i] - Z[u]) * (Z[i] - Z[u]))
                    if Distance < _autofragment_convert(u, symbol) + _autofragment_convert(i, symbol):
                        Queue.append(i)  # if you find you, put it in the que
                        White.remove(i)  # and remove it from the untouched list
            Queue.remove(u)  # remove focus from Queue
            Black.append(u)
            Fragment[frag].append(int(u + 1))  # add to group (adding 1 to start
                                           #   list at one instead of zero)

        if(len(White) != 0):  # cant move White->Queue if no more exist
            Queue.append(White[0])
            White.remove(White[0])
        frag += 1

    new_geom = """\n"""
    for i in Fragment[0]:
        new_geom = new_geom + F[i].lstrip() + """\n"""
    new_geom = new_geom + """--\n"""
    for j in Fragment[1]:
        new_geom = new_geom + F[j].lstrip() + """\n"""
    new_geom = new_geom + """units angstrom\n"""

    moleculenew = psi4.Molecule.create_molecule_from_string(new_geom)
    moleculenew.set_name(molname)
    moleculenew.update_geometry()
    moleculenew.print_cluster()
    psi4.print_out("""  Exiting auto_fragments\n""")

    return moleculenew

#######################
##  Start of n_body  ##
#######################


def n_body(name, **kwargs):
    lowername = name.lower()
    kwargs = p4util.kwargs_lower(kwargs)

    # Wrap any positional arguments into kwargs (for intercalls among wrappers)
    if not('name' in kwargs) and name:
        kwargs['name'] = name.lower()

    # Establish function to call
    func = kwargs.pop('n_body_func', kwargs.pop('func', energy))
    kwargs['n_body_func'] = func
    if func is db:
        raise ValidationError("""Wrapper n_body is unhappy to be calling function '%s'.""" % (func.__name__))

    # Make sure the molecule the user provided is the active one
    molecule = kwargs.pop('molecule', psi4.get_active_molecule())
    molecule.update_geometry()

    # N-body run configuration
    bsse = 'on'
    if 'bsse' in kwargs:
        bsse = kwargs['bsse']

    max_n_body = molecule.nfragments()
    if 'max_n_body' in kwargs:
        max_n_body = kwargs['max_n_body']

    do_total = False
    if 'do_total' in kwargs:
        do_total = kwargs['do_total']

    external = None
    external_indices = []
    if 'external' in kwargs:
        external = kwargs['external']
        external_indices = [molecule.nfragments()]
        if 'external_monomers' in kwargs:
            external_indices = kwargs['external_monomers']

    # Check input args
    if not bsse == 'off' and not bsse == 'on' and not bsse == 'both':
        raise ValidationError('n_body: bsse argument is one of on, off, or both')
    if max_n_body < 1:
        raise ValidationError('n_body: max_n_body must be at least 1')
    if max_n_body > molecule.nfragments():
        raise ValidationError('n_body: max_n_body must be <= to the number of fragments in the molecule')

    # Set to save RI integrals for repeated full-basis computations
    ri_ints_io = psi4.get_global_option('DF_INTS_IO')
    # inquire if above at all applies to dfmp2 or just scf
    psi4.set_global_option('DF_INTS_IO', 'SAVE')
    psioh = psi4.IOManager.shared_object()
    psioh.set_specific_retention(97, True)

    # Tell 'em what you're gonna tell 'em
    has_external = 'No'
    if (external):
        has_external = 'Yes'
    psi4.print_out('\n')
    psi4.print_out('    ==> N-Body Interaction Energy Analysis <==\n\n')
    psi4.print_out('        BSSE Treatment:              %s\n' % (bsse))
    psi4.print_out('        Maximum N-Body Interactions: %d\n' % (max_n_body))
    psi4.print_out('        Compute Total Energy:        %s\n' % (do_total))
    psi4.print_out('        External Field:              %s\n' % (has_external))
    if (external):
        psi4.print_out('        External Field Monomers:     ')
        for k in external_indices:
            psi4.print_out('%-3d ' % (k))
        psi4.print_out('\n')
    psi4.print_out('\n')

    # Run the total molecule, if required
    energies_full = {}
    energies_mon = {}
    N = molecule.nfragments()
    Etotal = 0.0
    if do_total or max_n_body == molecule.nfragments():
        psi4.print_out('    => Total Cluster Energy <=\n')
        # Full cluster always gets the external field
        if (external):
            psi4.set_global_option_python("EXTERN", external)
        Etotal = func(**kwargs)
        if (external):
            psi4.set_global_option_python("EXTERN", None)
        energies_full[N] = []
        energies_full[N].append(Etotal)
        energies_mon[N] = []
        energies_mon[N].append(Etotal)
        psi4.set_global_option('DF_INTS_IO', 'LOAD')
        psi4.clean()

    max_effective = max_n_body
    if (max_effective == N):
        max_effective = N - 1

    # Build the combos for indexing purposes
    Ns = []
    if (max_n_body == N or do_total):
        Ns.append(N)
    for n in range(max_effective, 0, -1):
        Ns.append(n)

    combos = {}
    for n in Ns:

        combos[n] = []

        # Loop through combinations in lexical order #

        # initialize the reals list
        reals = []
        #setup first combination [3,2,1] lexical ordering
        #fragments indexing is 1's based, bloody hell
        for index in range(n, 0, -1):
            reals.append(index)
        #start loop through lexical promotion
        counter = 0
        while True:

            counter = counter + 1

            # Append the current combo
            combos[n].append(copy.deepcopy(reals))

            #reset rank
            rank = 0

            #look for lexical promotion opportunity
            #i.e.: [4 2 1] has a promotion opportunity at
            # index 1 to produce [4 3 1]
            for k in range(n - 2, -1, -1):
                if (reals[k] != reals[k + 1] + 1):
                    rank = k + 1
                    break

            #do the promotion
            reals[rank] = reals[rank] + 1

            #demote the right portion of the register
            val = 1
            for k in range(n - 1, rank, -1):
                reals[k] = val
                val = val + 1

            #boundary condition is promotion into
            #[nfrag+1 nfrag-1 ...]
            if (reals[0] > N):
                break

    # Hack for external
    externNone = psi4.ExternalPotential()

    # Run the clusters in the full basis
    if bsse == 'on' or bsse == 'both':
        for n in range(max_effective, 0, -1):
            energies_full[n] = []
            clusters = extract_clusters(molecule, True, n)
            for k in range(len(clusters)):
                # Do the external field for this cluster or not?
                if (external):
                    do_extern = False
                    for mon in combos[n][k]:
                        if (mon in external_indices):
                            do_extern = True
                            break
                    if do_extern:
                        psi4.set_global_option_python("EXTERN", external)
                psi4.print_out('\n    => Cluster (N-Body %4d, Combination %4d) Energy (Full Basis) <=\n' % (n, k + 1))
                energies_full[n].append(func(molecule=clusters[k], **kwargs))
                # Turn the external field off
                if (external):
                    psi4.set_global_option_python("EXTERN", externNone)
                psi4.set_global_option('DF_INTS_IO', 'LOAD')
                psi4.clean()

    # Run the clusters in the minimal cluster bases
    psi4.set_global_option('DF_INTS_IO', 'NONE')
    if bsse == 'off' or bsse == 'both':
        for n in range(max_effective, 0, -1):
            energies_mon[n] = []
            clusters = extract_clusters(molecule, False, n)
            for k in range(len(clusters)):
                # Do the external field for this cluster or not?
                if (external):
                    do_extern = False
                    for mon in combos[n][k]:
                        if (mon in external_indices):
                            do_extern = True
                            break
                    if do_extern:
                        psi4.set_global_option_python("EXTERN", external)
                psi4.print_out('\n    => Cluster (N-Body %4d, Combination %4d) Energy (Cluster Basis) <=\n' % (n, k + 1))
                energies_mon[n].append(func(molecule=clusters[k], **kwargs))
                # Turn the external field off
                if (external):
                    psi4.set_global_option_python("EXTERN", externNone)
                psi4.clean()

    # Report the energies
    psi4.print_out('\n    ==> N-Body Interaction Energy Analysis: Combination Definitions <==\n\n')

    psi4.print_out('     %6s %6s | %-24s\n' % ("N-Body", "Combo", "Monomers"))
    for n in Ns:
        for k in range(len(combos[n])):
            psi4.print_out('     %6d %6d | ' % (n, k + 1))
            for l in combos[n][k]:
                psi4.print_out('%-3d ' % (l))
            psi4.print_out('\n')
    psi4.print_out('\n')

    psi4.print_out('    ==> N-Body Interaction Energy Analysis: Total Energies <==\n\n')

    if bsse == 'on' or bsse == 'both':
        psi4.print_out('     => Full Basis Set Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        for n in Ns:
            for k in range(len(energies_full[n])):
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, energies_full[n][k],
                   p4const.psi_hartree2kcalmol * energies_full[n][k],
                   p4const.psi_hartree2kJmol * energies_full[n][k]))
        psi4.print_out('\n')

    if bsse == 'off' or bsse == 'both':
        psi4.print_out('     => Cluster Basis Set Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        for n in Ns:
            for k in range(len(energies_mon[n])):
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, energies_mon[n][k],
                   p4const.psi_hartree2kcalmol * energies_mon[n][k],
                   p4const.psi_hartree2kJmol * energies_mon[n][k]))
        psi4.print_out('\n')

    if bsse == 'both':
        psi4.print_out('     => BSSE Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "Delta E [Eh]", "Delta E [kcal/mol]", "Delta E [kJ/mol]"))
        for n in Ns:
            for k in range(len(energies_mon[n])):
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, energies_full[n][k] - energies_mon[n][k],
                   p4const.psi_hartree2kcalmol * (energies_full[n][k] - energies_mon[n][k]),
                   p4const.psi_hartree2kJmol * (energies_full[n][k] - energies_mon[n][k])))
        psi4.print_out('\n')

    psi4.print_out('    ==> N-Body Interaction Energy Analysis: N-Body Energies <==\n\n')

    if bsse == 'on' or bsse == 'both':
        psi4.print_out('     => Full Basis Set Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        energies_n_full = {}
        for n in Ns:
            if n == 1:
                continue
            En = 0.0
            for k in range(len(energies_full[n])):
                E = energies_full[n][k]
                for l in range(len(combos[n][k])):
                    E -= energies_full[1][combos[n][k][l] - 1]
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, E, 
                    p4const.psi_hartree2kcalmol * E,
                    p4const.psi_hartree2kJmol * E))
                En += E
            energies_n_full[n] = En
        for n in Ns:
            if n == 1:
                continue
            nn = molecule.nfragments() - 2
            kk = n - 2
            energies_n_full[n] /= (math.factorial(nn) / (math.factorial(kk) * math.factorial(nn - kk)))
            psi4.print_out('     %6d %6s %24.16E %24.16E %24.16E\n' % (n, 'Total', energies_n_full[n],
               p4const.psi_hartree2kcalmol * energies_n_full[n],
               p4const.psi_hartree2kJmol * energies_n_full[n]))
        psi4.print_out('\n')

    if bsse == 'off' or bsse == 'both':
        psi4.print_out('     => Cluster Basis Set Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        energies_n_mon = {}
        for n in Ns:
            if n == 1:
                continue
            En = 0.0
            for k in range(len(energies_mon[n])):
                E = energies_mon[n][k]
                for l in range(len(combos[n][k])):
                    E -= energies_mon[1][combos[n][k][l] - 1]
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, E,
                    p4const.psi_hartree2kcalmol * E,
                    p4const.psi_hartree2kJmol * E))
                En += E
            energies_n_mon[n] = En
        for n in Ns:
            if n == 1:
                continue
            nn = molecule.nfragments() - 2
            kk = n - 2
            energies_n_mon[n] /= (math.factorial(nn) / (math.factorial(kk) * math.factorial(nn - kk)))
            psi4.print_out('     %6d %6s %24.16E %24.16E %24.16E\n' % (n, 'Total', energies_n_mon[n],
               p4const.psi_hartree2kcalmol * energies_n_mon[n],
               p4const.psi_hartree2kJmol * energies_n_mon[n]))
        psi4.print_out('\n')

    if bsse == 'both':
        psi4.print_out('     => BSSE Results <=\n\n')
        psi4.print_out('     %6s %6s %24s %24s %24s\n' % ("N-Body", "Combo", "Delta E [Eh]", "Delta E [kcal/mol]", "Delta E [kJ/mol]"))
        energies_n_bsse = {}
        for n in Ns:
            if n == 1:
                continue
            En = 0.0
            for k in range(len(energies_mon[n])):
                E = energies_full[n][k] - energies_mon[n][k]
                for l in range(len(combos[n][k])):
                    E -= energies_full[1][combos[n][k][l] - 1]
                    E += energies_mon[1][combos[n][k][l] - 1]
                psi4.print_out('     %6d %6d %24.16E %24.16E %24.16E\n' % (n, k + 1, E,
                    p4const.psi_hartree2kcalmol * E,
                    p4const.psi_hartree2kJmol * E))
                En += E
            energies_n_bsse[n] = En
        for n in Ns:
            if n == 1:
                continue
            nn = molecule.nfragments() - 2
            kk = n - 2
            energies_n_bsse[n] /= (math.factorial(nn) / (math.factorial(kk) * math.factorial(nn - kk)))
            psi4.print_out('     %6d %6s %24.16E %24.16E %24.16E\n' % (n, 'Total', energies_n_bsse[n],
               p4const.psi_hartree2kcalmol * energies_n_bsse[n],
               p4const.psi_hartree2kJmol * energies_n_bsse[n]))
        psi4.print_out('\n')

    psi4.print_out('    ==> N-Body Interaction Energy Analysis: Non-Additivities <==\n\n')

    if bsse == 'on' or bsse == 'both':
        energies_n_full[1] = 0.0
        psi4.print_out('     => Full Basis Set Results <=\n\n')
        psi4.print_out('     %6s %24s %24s %24s\n' % ("N-Body", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        for k in range(len(Ns)):
            n = Ns[k]
            if n == 1:
                continue
            E = energies_n_full[Ns[k]] - energies_n_full[Ns[k + 1]]
            psi4.print_out('     %6s %24.16E %24.16E %24.16E\n' % (n, E,
                p4const.psi_hartree2kcalmol * E,
                p4const.psi_hartree2kJmol * E))
        psi4.print_out('\n')

    if bsse == 'off' or bsse == 'both':
        energies_n_mon[1] = 0.0
        psi4.print_out('     => Cluster Basis Set Results <=\n\n')
        psi4.print_out('     %6s %24s %24s %24s\n' % ("N-Body", "E [Eh]", "E [kcal/mol]", "E [kJ/mol]"))
        for k in range(len(Ns)):
            n = Ns[k]
            if n == 1:
                continue
            E = energies_n_mon[Ns[k]] - energies_n_mon[Ns[k + 1]]
            psi4.print_out('     %6s %24.16E %24.16E %24.16E\n' % (n, E,
                p4const.psi_hartree2kcalmol * E,
                p4const.psi_hartree2kJmol * E))
        psi4.print_out('\n')

    if bsse == 'both':
        energies_n_bsse[1] = 0.0
        psi4.print_out('     => BSSE Results <=\n\n')
        psi4.print_out('     %6s %24s %24s %24s\n' % ("N-Body", "Delta E [Eh]", "Delta E [kcal/mol]", "Delta E [kJ/mol]"))
        for k in range(len(Ns)):
            n = Ns[k]
            if n == 1:
                continue
            E = energies_n_bsse[Ns[k]] - energies_n_bsse[Ns[k + 1]]
            psi4.print_out('     %6s %24.16E %24.16E %24.16E\n' % (n, E,
                p4const.psi_hartree2kcalmol * E,
                p4const.psi_hartree2kJmol * E))
        psi4.print_out('\n')

    # Put everything back the way it was
    psi4.set_global_option('DF_INTS_IO', ri_ints_io)
    psioh.set_specific_retention(97, False)
    psi4.clean()

    if bsse == 'on' or bsse == 'both':
        return energies_n_full[Ns[0]]
    else:
        return energies_n_mon[Ns[0]]

##  Aliases  ##
nbody = n_body

#####################
##  End of n_body  ##
#####################


###################
##  Start of cp  ##
###################

def cp(name, **kwargs):
    r"""The cp function computes counterpoise-corrected two-body interaction energies
    for complexes composed of arbitrary numbers of monomers.

    :aliases: counterpoise_correct(), counterpoise_correction()

    :returns: (*float*) Counterpoise-corrected interaction energy in Hartrees.

    :PSI variables:

    .. hlist::
       :columns: 1

       * :psivar:`CP-CORRECTED 2-BODY INTERACTION ENERGY <CP-CORRECTED2-BODYINTERACTIONENERGY>`
       * :psivar:`UNCP-CORRECTED 2-BODY INTERACTION ENERGY <UNCP-CORRECTED2-BODYINTERACTIONENERGY>`

    .. caution:: Some features are not yet implemented. Buy a developer a coffee.

       - No values of func besides energy have been tested.

       - Table print-out needs improving. Add some PSI variables.

    :type name: string
    :param name: ``'scf'`` || ``'ccsd(t)'`` || etc.

        First argument, usually unlabeled. Indicates the computational method
        to be applied to the molecule. May be any valid argument to
        :py:func:`~driver.energy`; however, SAPT is not appropriate.

    :type func: :ref:`function <op_py_function>`
    :param func: |dl| ``energy`` |dr| || ``optimize`` || ``cbs``

        Indicates the type of calculation to be performed on the molecule
        and each of its monomers. The default performs a single-point
        ``energy('name')``, while ``optimize`` perfoms a geometry optimization
        on each system, and ``cbs`` performs a compound single-point energy.
        If a nested series of python functions is intended
        (see :ref:`sec:intercalls`), use keyword ``cp_func`` instead of ``func``.

    :type check_bsse: :ref:`boolean <op_py_boolean>`
    :param check_bsse: ``'on'`` || |dl| ``'off'`` |dr|

        Indicates whether to additionally compute un-counterpoise corrected
        monomers and thus obtain an estimate for the basis set superposition error.

    :type molecule: :ref:`molecule <op_py_molecule>`
    :param molecule: ``h2o`` || etc.

        The target molecule, if not the last molecule defined.

    :examples:

    >>> # [1] counterpoise-corrected density-fitted mp2 interaction energy
    >>> cp('mp2')

    """
    lowername = name.lower()
    kwargs = p4util.kwargs_lower(kwargs)

    # Wrap any positional arguments into kwargs (for intercalls among wrappers)
    if not('name' in kwargs) and name:
        kwargs['name'] = name.lower()

    # Establish function to call
    func = kwargs.pop('cp_func', kwargs.pop('func', energy))
    kwargs['cp_func'] = func
    if func is db:
        raise ValidationError("""Wrapper cp is unhappy to be calling function '%s'.""" % (func.__name__))

    if 'check_bsse' in kwargs and yes.match(str(kwargs['check_bsse'])):
        check_bsse = True
    else:
        check_bsse = False

    # Make sure the molecule the user provided is the active one
    molecule = kwargs.pop('molecule', psi4.get_active_molecule())
    molecule.update_geometry()

    df_ints_io = psi4.get_global_option('DF_INTS_IO')
    # inquire if above at all applies to dfmp2 or just scf
    psi4.set_global_option('DF_INTS_IO', 'SAVE')
    psioh = psi4.IOManager.shared_object()
    psioh.set_specific_retention(97, True)

    psi4.print_out("\n")
    p4util.banner("CP Computation: Complex.\nFull Basis Set.")
    psi4.print_out("\n")

    e_dimer = func(molecule=molecule, **kwargs)

    psi4.clean()
    psi4.set_global_option('DF_INTS_IO', 'LOAD')

    # All monomers with ghosts
    monomers = extract_clusters(molecule, True, 1)
    e_monomer_full = []

    cluster_n = 0
    for cluster in monomers:
        psi4.print_out("\n")
        p4util.banner(("CP Computation: Monomer %d.\n Full Basis Set." % (cluster_n + 1)))
        psi4.print_out("\n")
        e_monomer_full.append(func(molecule=cluster, **kwargs))
        cluster_n = cluster_n + 1
        psi4.clean()

    psi4.set_global_option('DF_INTS_IO', 'NONE')
    if check_bsse:
        # All monomers without ghosts
        monomers = extract_clusters(molecule, False, 1)
        e_monomer_bsse = []

        cluster_n = 0
        for cluster in monomers:
            psi4.print_out("\n")
            p4util.banner(("CP Computation: Monomer %d.\n Monomer Set." % (cluster_n + 1)))
            psi4.print_out("\n")
            e_monomer_bsse.append(func(molecule=cluster, **kwargs))
            cluster_n = cluster_n + 1

    psi4.set_global_option('DF_INTS_IO', df_ints_io)
    psioh.set_specific_retention(97, False)

    if not check_bsse:
        cp_table = p4util.Table(rows=["System:"], cols=["Energy (full):"], width=24)
        cp_table["Complex"] = [e_dimer]
        for cluster_n in range(0, len(monomers)):
            key = "Monomer %d" % (cluster_n + 1)
            cp_table[key] = [e_monomer_full[cluster_n]]

        e_full = e_dimer
        for cluster_n in range(0, len(monomers)):
            e_full = e_full - e_monomer_full[cluster_n]
        cp_table["Interaction"] = [e_full]

        psi4.set_variable('CP-CORRECTED 2-BODY INTERACTION ENERGY', e_full)

    else:
        cp_table = Table(rows=["System:"], cols=["Energy (full):", "Energy (monomer):", "BSSE:"], width=24)
        cp_table["Complex"] = [e_dimer, 0.0, 0.0]
        for cluster_n in range(0, len(monomers)):
            key = "Monomer %d" % (cluster_n + 1)
            cp_table[key] = [e_monomer_full[cluster_n], e_monomer_bsse[cluster_n], \
                e_monomer_full[cluster_n] - e_monomer_bsse[cluster_n]]

        e_full = e_dimer
        e_bsse = e_dimer
        for cluster_n in range(0, len(monomers)):
            e_full = e_full - e_monomer_full[cluster_n]
            e_bsse = e_bsse - e_monomer_bsse[cluster_n]
        cp_table["Totals:"] = [e_full, e_bsse, e_full - e_bsse]

        psi4.set_variable('UNCP-CORRECTED 2-BODY INTERACTION ENERGY', e_full)

    psi4.print_out("\n")
    p4util.banner("CP Computation: Results.")
    psi4.print_out("\n")

    p4util.banner("[Eh]", 2)
    psi4.print_out("\n")

    psi4.print_out(str(cp_table))

    psi4.print_out("\n")
    p4util.banner("[kcal/mol]", 2)
    psi4.print_out("\n")

    cp_table.scale()

    psi4.print_out(str(cp_table))

    psi4.print_out("\n")
    p4util.banner("[kJ/mol]", 2)
    psi4.print_out("\n")

    cp_table.scale(Factor=p4const.psi_hartree2kJmol/p4const.psi_hartree2kcalmol)

    psi4.print_out(str(cp_table))
    return e_full

##  Aliases  ##
counterpoise_correct = cp
counterpoise_correction = cp

#################
##  End of cp  ##
#################


#########################
##  Start of Database  ##
#########################

DB_RGT = {}
DB_RXN = {}


def database(name, db_name, **kwargs):
    r"""Function to access the molecule objects and reference energies of
    popular chemical databases.

    :aliases: db()

    :returns: (*float*) Mean absolute deviation of the database in kcal/mol

    :PSI variables:

    .. hlist::
       :columns: 1

       * :psivar:`db_name DATABASE MEAN SIGNED DEVIATION <db_nameDATABASEMEANSIGNEDDEVIATION>`
       * :psivar:`db_name DATABASE MEAN ABSOLUTE DEVIATION <db_nameDATABASEMEANABSOLUTEDEVIATION>`
       * :psivar:`db_name DATABASE ROOT-MEAN-SQUARE DEVIATION <db_nameDATABASEROOT-MEAN-SQUARESIGNEDDEVIATION>`
       * Python dictionaries of results accessible as ``DB_RGT`` and ``DB_RXN``.

    .. note:: It is very easy to make a database from a collection of xyz files
        using the script :source:`share/scripts/ixyz2database.pl`.
        See :ref:`sec:createDatabase` for details.

    .. caution:: Some features are not yet implemented. Buy a developer some coffee.

       - In sow/reap mode, use only global options (e.g., the local option set by ``set scf scf_type df`` will not be respected).

    .. note:: To access a database that is not embedded in a |PSIfour|
        distribution, add the path to the directory containing the database
        to the environment variable :envvar:`PYTHONPATH`.

    :type name: string
    :param name: ``'scf'`` || ``'sapt0'`` || ``'ccsd(t)'`` || etc.

        First argument, usually unlabeled. Indicates the computational method
        to be applied to the database. May be any valid argument to
        :py:func:`~driver.energy`.

    :type db_name: string
    :param db_name: ``'BASIC'`` || ``'S22'`` || ``'HTBH'`` || etc.

        Second argument, usually unlabeled. Indicates the requested database
        name, matching (case insensitive) the name of a python file in
        ``psi4/share/databases`` or :envvar:`PYTHONPATH`.  Consult that
        directory for available databases and literature citations.

    :type func: :ref:`function <op_py_function>`
    :param func: |dl| ``energy`` |dr| || ``optimize`` || ``cbs``

        Indicates the type of calculation to be performed on each database
        member. The default performs a single-point ``energy('name')``, while
        ``optimize`` perfoms a geometry optimization on each reagent, and
        ``cbs`` performs a compound single-point energy. If a nested series
        of python functions is intended (see :ref:`sec:intercalls`), use
        keyword ``db_func`` instead of ``func``.

    :type mode: string
    :param mode: |dl| ``'continuous'`` |dr| || ``'sow'`` || ``'reap'``

        Indicates whether the calculations required to complete the
        database are to be run in one file (``'continuous'``) or are to be
        farmed out in an embarrassingly parallel fashion
        (``'sow'``/``'reap'``).  For the latter, run an initial job with
        ``'sow'`` and follow instructions in its output file.

    :type cp: :ref:`boolean <op_py_boolean>`
    :param cp: ``'on'`` || |dl| ``'off'`` |dr|

        Indicates whether counterpoise correction is employed in computing
        interaction energies. Use this option and NOT the :py:func:`~wrappers.cp`
        function for BSSE correction in database().  Option available
        (See :ref:`sec:availableDatabases`) only for databases of bimolecular complexes.

    :type rlxd: :ref:`boolean <op_py_boolean>`
    :param rlxd: ``'on'`` || |dl| ``'off'`` |dr|

        Indicates whether correction for deformation energy is
        employed in computing interaction energies.  Option available
        (See :ref:`sec:availableDatabases`) only for databases of bimolecular complexes
        with non-frozen monomers, e.g., HBC6.

    :type symm: :ref:`boolean <op_py_boolean>`
    :param symm: |dl| ``'on'`` |dr| || ``'off'``

        Indicates whether the native symmetry of the database reagents is
        employed (``'on'``) or whether it is forced to :math:`C_1` symmetry
        (``'off'``). Some computational methods (e.g., SAPT) require no
        symmetry, and this will be set by database().

    :type zpe: :ref:`boolean <op_py_boolean>`
    :param zpe: ``'on'`` || |dl| ``'off'`` |dr|

        Indicates whether zero-point-energy corrections are appended to
        single-point energy values. Option valid only for certain
        thermochemical databases. Disabled until Hessians ready.

    :type benchmark: string
    :param benchmark: |dl| ``'default'`` |dr| || ``'S22A'`` || etc.

        Indicates whether a non-default set of reference energies, if
        available (See :ref:`sec:availableDatabases`), are employed for the
        calculation of error statistics.

    :type tabulate: array of strings
    :param tabulate: |dl| ``[]`` |dr| || ``['scf total energy', 'natom']`` || etc.

        Indicates whether to form tables of variables other than the
        primary requested energy.  Available for any PSI variable.

    :type subset: string or array of strings
    :param subset:

        Indicates a subset of the full database to run. This is a very
        flexible option and can be used in three distinct ways, outlined
        below. Note that two take a string and the last takes an array.
        See `Available Databases`_ for available values.

        * ``'small'`` || ``'large'`` || ``'equilibrium'``
            Calls predefined subsets of the requested database, either
            ``'small'``, a few of the smallest database members,
            ``'large'``, the largest of the database members, or
            ``'equilibrium'``, the equilibrium geometries for a database
            composed of dissociation curves.
        * ``'BzBz_S'`` || ``'FaOOFaON'`` || ``'ArNe'`` ||  ``'HB'`` || etc.
            For databases composed of dissociation curves, or otherwise
            divided into subsets, individual curves and subsets can be
            called by name. Consult the database python files for available
            molecular systems (case insensitive).
        * ``[1,2,5]`` || ``['1','2','5']`` || ``['BzMe-3.5', 'MeMe-5.0']`` || etc.
            Specify a list of database members to run. Consult the
            database python files for available molecular systems.  This
            is the only portion of database input that is case sensitive;
            choices for this keyword must match the database python file.

    :examples:

    >>> # [1] Two-stage SCF calculation on short, equilibrium, and long helium dimer
    >>> db('scf','RGC10',cast_up='sto-3g',subset=['HeHe-0.85','HeHe-1.0','HeHe-1.5'], tabulate=['scf total energy','natom'])

    >>> # [2] Counterpoise-corrected interaction energies for three complexes in S22
    >>> #     Error statistics computed wrt an old benchmark, S22A
    >>> database('mp2','S22',cp=1,subset=[16,17,8],benchmark='S22A')

    >>> # [3] SAPT0 on the neon dimer dissociation curve
    >>> db('sapt0',subset='NeNe',cp=0,symm=0,db_name='RGC10')

    >>> # [4] Optimize system 1 in database S22, producing tables of scf and mp2 energy
    >>> db('mp2','S22',db_func=optimize,subset=[1], tabulate=['mp2 total energy','current energy'])

    >>> # [5] CCSD on the smallest systems of HTBH, a hydrogen-transfer database
    >>> database('ccsd','HTBH',subset='small', tabulate=['ccsd total energy', 'mp2 total energy'])

    """
    lowername = name.lower()
    kwargs = p4util.kwargs_lower(kwargs)

    # Wrap any positional arguments into kwargs (for intercalls among wrappers)
    if not('name' in kwargs) and name:
        kwargs['name'] = name.lower()
    if not('db_name' in kwargs) and db_name:
        kwargs['db_name'] = db_name

    # Establish function to call
    func = kwargs.pop('db_func', kwargs.pop('func', energy))
    kwargs['db_func'] = func
    if func is cp:
        raise ValidationError("""Wrapper database is unhappy to be calling function '%s'. Use the cp keyword within database instead'.""" % (func.__name__))

    optstash = p4util.OptionsState(
        ['WRITER_FILE_LABEL'],
        ['SCF', 'REFERENCE'])

    # Wrapper wholly defines molecule. discard any passed-in
    kwargs.pop('molecule', None)

    # Paths to search for database files: here + PSIPATH + library + PYTHONPATH
    psidatadir = os.environ.get('PSIDATADIR', None)
    psidatadir = __file__ + '/../..' if psidatadir is None else psidatadir
    libraryPath = ':' + os.path.abspath(psidatadir) + '/databases'
    dbPath = os.path.abspath('.') + \
        ':' + ':'.join([os.path.abspath(x) for x in os.environ.get('PSIPATH', '').split(':')]) + \
        libraryPath
    sys.path = [sys.path[0]] + dbPath.split(':') + sys.path[1:]
    # TODO this should be modernized a la interface_cfour

    # Define path and load module for requested database
    database = p4util.import_ignorecase(db_name)
    if database is None:
        psi4.print_out('\nPython module for database %s failed to load\n\n' % (db_name))
        psi4.print_out('\nSearch path that was tried:\n')
        psi4.print_out(", ".join(map(str, sys.path)))
        raise ValidationError("Python module loading problem for database " + str(db_name))
    else:
        dbse = database.dbse
        HRXN = database.HRXN
        ACTV = database.ACTV
        RXNM = database.RXNM
        BIND = database.BIND
        TAGL = database.TAGL
        GEOS = database.GEOS
        try:
            DATA = database.DATA
        except AttributeError:
            DATA = {}

    user_writer_file_label = psi4.get_global_option('WRITER_FILE_LABEL')
    user_reference = psi4.get_global_option('REFERENCE')

    # Configuration based upon e_name & db_name options
    #   Force non-supramolecular if needed
    if re.match(r'^sapt', lowername) or re.match(r'^.*sapt', lowername):
        try:
            database.ACTV_SA
        except AttributeError:
            raise ValidationError('Database %s not suitable for non-supramolecular calculation.' % (db_name))
        else:
            ACTV = database.ACTV_SA
    #   Force open-shell if needed
    openshell_override = 0
    if (user_reference == 'RHF') or (user_reference == 'RKS'):
        try:
            database.isOS
        except AttributeError:
            pass
        else:
            if yes.match(str(database.isOS)):
                openshell_override = 1
                psi4.print_out('\nSome reagents in database %s require an open-shell reference; will be reset to UHF/UKS as needed.\n' % (db_name))

    # Configuration based upon database keyword options
    #   Option symmetry- whether symmetry treated normally or turned off (currently req'd for dfmp2 & dft)
    db_symm = 'yes'
    if 'symm' in kwargs:
        db_symm = kwargs['symm']

    symmetry_override = 0
    if no.match(str(db_symm)):
        symmetry_override = 1
    elif yes.match(str(db_symm)):
        pass
    else:
        raise ValidationError('Symmetry mode \'%s\' not valid.' % (db_symm))

    #   Option mode of operation- whether db run in one job or files farmed out
    db_mode = kwargs.pop('db_mode', kwargs.pop('mode', 'continuous')).lower()
    kwargs['db_mode'] = db_mode

    if db_mode == 'continuous':
        pass
    elif db_mode == 'sow':
        pass
    elif db_mode == 'reap':
        db_linkage = kwargs.get('linkage', None)
        if db_linkage is None:
            raise ValidationError("""Database execution mode 'reap' requires a linkage option.""")
    else:
        raise ValidationError("""Database execution mode '%s' not valid.""" % (db_mode))

    #   Option counterpoise- whether for interaction energy databases run in bsse-corrected or not
    db_cp = 'no'
    if 'cp' in kwargs:
        db_cp = kwargs['cp']

    if yes.match(str(db_cp)):
        try:
            database.ACTV_CP
        except AttributeError:
            raise ValidationError('Counterpoise correction mode \'yes\' invalid for database %s.' % (db_name))
        else:
            ACTV = database.ACTV_CP
    elif no.match(str(db_cp)):
        pass
    else:
        raise ValidationError('Counterpoise correction mode \'%s\' not valid.' % (db_cp))

    #   Option relaxed- whether for non-frozen-monomer interaction energy databases include deformation correction or not?
    db_rlxd = 'no'
    if 'rlxd' in kwargs:
        db_rlxd = kwargs['rlxd']

    if yes.match(str(db_rlxd)):
        if yes.match(str(db_cp)):
            try:
                database.ACTV_CPRLX
                database.RXNM_CPRLX
            except AttributeError:
                raise ValidationError('Deformation and counterpoise correction mode \'yes\' invalid for database %s.' % (db_name))
            else:
                ACTV = database.ACTV_CPRLX
                RXNM = database.RXNM_CPRLX
        elif no.match(str(db_cp)):
            try:
                database.ACTV_RLX
            except AttributeError:
                raise ValidationError('Deformation correction mode \'yes\' invalid for database %s.' % (db_name))
            else:
                ACTV = database.ACTV_RLX
    elif no.match(str(db_rlxd)):
        pass
    else:
        raise ValidationError('Deformation correction mode \'%s\' not valid.' % (db_rlxd))

    #   Option zero-point-correction- whether for thermochem databases jobs are corrected by zpe
    db_zpe = 'no'
    if 'zpe' in kwargs:
        db_zpe = kwargs['zpe']

    if yes.match(str(db_zpe)):
        raise ValidationError('Zero-point-correction mode \'yes\' not yet implemented.')
    elif no.match(str(db_zpe)):
        pass
    else:
        raise ValidationError('Zero-point-correction \'mode\' %s not valid.' % (db_zpe))

    #   Option benchmark- whether error statistics computed wrt alternate reference energies
    db_benchmark = 'default'
    if 'benchmark' in kwargs:
        db_benchmark = kwargs['benchmark']

        if (db_benchmark.lower() == 'default'):
            pass
        else:
            BIND = p4util.getattr_ignorecase(database, 'BIND_' + db_benchmark)
            if BIND is None:
                raise ValidationError('Special benchmark \'%s\' not available for database %s.' % (db_benchmark, db_name))

    #   Option tabulate- whether tables of variables other than primary energy method are formed
    # TODO db(func=cbs,tabulate=[non-current-energy])  # broken
    db_tabulate = []
    if 'tabulate' in kwargs:
        db_tabulate = kwargs['tabulate']

    #   Option subset- whether all of the database or just a portion is run
    db_subset = HRXN
    if 'subset' in kwargs:
        db_subset = kwargs['subset']

    if isinstance(db_subset, basestring):
        if (db_subset.lower() == 'small'):
            try:
                database.HRXN_SM
            except AttributeError:
                raise ValidationError('Special subset \'small\' not available for database %s.' % (db_name))
            else:
                HRXN = database.HRXN_SM
        elif (db_subset.lower() == 'large'):
            try:
                database.HRXN_LG
            except AttributeError:
                raise ValidationError('Special subset \'large\' not available for database %s.' % (db_name))
            else:
                HRXN = database.HRXN_LG
        elif (db_subset.lower() == 'equilibrium'):
            try:
                database.HRXN_EQ
            except AttributeError:
                raise ValidationError('Special subset \'equilibrium\' not available for database %s.' % (db_name))
            else:
                HRXN = database.HRXN_EQ
        else:
            HRXN = p4util.getattr_ignorecase(database, db_subset)
            if HRXN is None:
                HRXN = p4util.getattr_ignorecase(database, 'HRXN_' + db_subset)
                if HRXN is None:
                    raise ValidationError('Special subset \'%s\' not available for database %s.' % (db_subset, db_name))
    else:
        temp = []
        for rxn in db_subset:
            if rxn in HRXN:
                temp.append(rxn)
            else:
                raise ValidationError('Subset element \'%s\' not a member of database %s.' % (str(rxn), db_name))
        HRXN = temp

    temp = []
    for rxn in HRXN:
        temp.append(ACTV['%s-%s' % (dbse, rxn)])
    HSYS = p4util.drop_duplicates(sum(temp, []))

    # Sow all the necessary reagent computations
    psi4.print_out("\n\n")
    p4util.banner(("Database %s Computation" % (db_name)))
    psi4.print_out("\n")

    #   write index of calcs to output file
    if db_mode == 'continuous':
        instructions = """\n    The database single-job procedure has been selected through mode='continuous'.\n"""
        instructions += """    Calculations for the reagents will proceed in the order below and will be followed\n"""
        instructions += """    by summary results for the database.\n\n"""
        for rgt in HSYS:
            instructions += """                    %-s\n""" % (rgt)
        instructions += """\n    Alternatively, a farming-out of the database calculations may be accessed through\n"""
        instructions += """    the database wrapper option mode='sow'/'reap'.\n\n"""
        psi4.print_out(instructions)

    #   write sow/reap instructions and index of calcs to output file and reap input file
    if db_mode == 'sow':
        instructions = """\n    The database sow/reap procedure has been selected through mode='sow'. In addition\n"""
        instructions += """    to this output file (which contains no quantum chemical calculations), this job\n"""
        instructions += """    has produced a number of input files (%s-*.in) for individual database members\n""" % (dbse)
        instructions += """    and a single input file (%s-master.in) with a database(mode='reap') command.\n""" % (dbse)
        instructions += """    The former may look very peculiar since processed and pickled python rather than\n"""
        instructions += """    raw input is written. Follow the instructions below to continue.\n\n"""
        instructions += """    (1)  Run all of the %s-*.in input files on any variety of computer architecture.\n""" % (dbse)
        instructions += """       The output file names must be as given below.\n\n"""
        for rgt in HSYS:
            instructions += """             psi4 -i %-27s -o %-27s\n""" % (rgt + '.in', rgt + '.out')
        instructions += """\n    (2)  Gather all the resulting output files in a directory. Place input file\n"""
        instructions += """         %s-master.in into that directory and run it. The job will be trivial in\n""" % (dbse)
        instructions += """         length and give summary results for the database in its output file.\n\n"""
        instructions += """             psi4 -i %-27s -o %-27s\n\n""" % (dbse + '-master.in', dbse + '-master.out')
        instructions += """    Alternatively, a single-job execution of the database may be accessed through\n"""
        instructions += """    the database wrapper option mode='continuous'.\n\n"""
        psi4.print_out(instructions)

        with open('%s-master.in' % (dbse), 'w') as fmaster:
            fmaster.write('# This is a psi4 input file auto-generated from the database() wrapper.\n\n')
            fmaster.write("database('%s', '%s', mode='reap', cp='%s', rlxd='%s', zpe='%s', benchmark='%s', linkage=%d, subset=%s, tabulate=%s)\n\n" %
                (name, db_name, db_cp, db_rlxd, db_zpe, db_benchmark, os.getpid(), HRXN, db_tabulate))

    #   Loop through chemical systems
    ERGT = {}
    ERXN = {}
    VRGT = {}
    VRXN = {}
    for rgt in HSYS:
        VRGT[rgt] = {}

        # build string of title banner
        banners = ''
        banners += """psi4.print_out('\\n')\n"""
        banners += """p4util.banner(' Database %s Computation: Reagent %s \\n   %s')\n""" % (db_name, rgt, TAGL[rgt])
        banners += """psi4.print_out('\\n')\n\n"""

        # build string of lines that defines contribution of rgt to each rxn
        actives = ''
        actives += """psi4.print_out('   Database Contributions Map:\\n   %s\\n')\n""" % ('-' * 75)
        for rxn in HRXN:
            db_rxn = dbse + '-' + str(rxn)
            if rgt in ACTV[db_rxn]:
                actives += """psi4.print_out('   reagent %s contributes by %.4f to reaction %s\\n')\n""" \
                   % (rgt, RXNM[db_rxn][rgt], db_rxn)
        actives += """psi4.print_out('\\n')\n\n"""

        # build string of commands for options from the input file  TODO: handle local options too
        commands = ''
        commands += """\npsi4.set_memory(%s)\n\n""" % (psi4.get_memory())
        for chgdopt in psi4.get_global_option_list():
            if psi4.has_global_option_changed(chgdopt):
                chgdoptval = psi4.get_global_option(chgdopt)
                #chgdoptval = psi4.get_option(chgdopt)
                if isinstance(chgdoptval, basestring):
                    commands += """psi4.set_global_option('%s', '%s')\n""" % (chgdopt, chgdoptval)
                elif isinstance(chgdoptval, int) or isinstance(chgdoptval, float):
                    commands += """psi4.set_global_option('%s', %s)\n""" % (chgdopt, chgdoptval)
                else:
                    pass
                    #raise ValidationError('Option \'%s\' is not of a type (string, int, float, bool) that can be processed by database wrapper.' % (chgdopt))

        # build string of molecule and commands that are dependent on the database
        commands += '\n'

        if symmetry_override:
            commands += """molecule.reset_point_group('c1')\n"""
            commands += """molecule.fix_orientation(True)\n"""
            commands += """molecule.fix_com(True)\n"""
            commands += """molecule.update_geometry()\n"""

        if (openshell_override) and (molecule.multiplicity() != 1):
            if user_reference == 'RHF':
                commands += """psi4.set_global_option('REFERENCE', 'UHF')\n"""
            elif user_reference == 'RKS':
                commands += """psi4.set_global_option('REFERENCE', 'UKS')\n"""

        commands += """psi4.set_global_option('WRITER_FILE_LABEL', '%s')\n""" % \
            (user_writer_file_label + ('' if user_writer_file_label == '' else '-') + rgt)

        # all modes need to step through the reagents but all for different purposes
        # continuous: defines necessary commands, executes energy(method) call, and collects results into dictionary
        # sow: opens individual reagent input file, writes the necessary commands, and writes energy(method) call
        # reap: opens individual reagent output file, collects results into a dictionary
        if db_mode == 'continuous':
            exec(banners)

            molecule = psi4.Molecule.create_molecule_from_string(GEOS[rgt].create_psi4_string_from_molecule())
            molecule.set_name(rgt)
            molecule.update_geometry()

            exec(commands)
            #print 'MOLECULE LIVES %23s %8s %4d %4d %4s' % (rgt, psi4.get_global_option('REFERENCE'),
            #    molecule.molecular_charge(), molecule.multiplicity(), molecule.schoenflies_symbol())
            ERGT[rgt] = func(molecule=molecule, **kwargs)
            psi4.print_variables()
            exec(actives)
            for envv in db_tabulate:
                VRGT[rgt][envv.upper()] = psi4.get_variable(envv)
            psi4.set_global_option("REFERENCE", user_reference)
            psi4.clean()
            #psi4.opt_clean()
            psi4.clean_variables()

        elif db_mode == 'sow':
            with open('%s.in' % (rgt), 'w') as freagent:
                freagent.write('# This is a psi4 input file auto-generated from the database() wrapper.\n\n')
                freagent.write(banners)
                freagent.write(p4util.format_molecule_for_input(GEOS[rgt], 'dbmol'))

                freagent.write(commands)
                freagent.write('''\npickle_kw = ("""''')
                pickle.dump(kwargs, freagent)
                freagent.write('''""")\n''')
                freagent.write("""\nkwargs = pickle.loads(pickle_kw)\n""")
                freagent.write("""electronic_energy = %s(**kwargs)\n\n""" % (func.__name__))
                freagent.write("""psi4.print_variables()\n""")
                freagent.write("""psi4.print_out('\\nDATABASE RESULT: computation %d for reagent %s """
                    % (os.getpid(), rgt))
                freagent.write("""yields electronic energy %20.12f\\n' % (electronic_energy))\n\n""")
                freagent.write("""psi4.set_variable('NATOM', dbmol.natom())\n""")
                for envv in db_tabulate:
                    freagent.write("""psi4.print_out('DATABASE RESULT: computation %d for reagent %s """
                        % (os.getpid(), rgt))
                    freagent.write("""yields variable value    %20.12f for variable %s\\n' % (psi4.get_variable(""")
                    freagent.write("""'%s'), '%s'))\n""" % (envv.upper(), envv.upper()))

        elif db_mode == 'reap':
            ERGT[rgt] = 0.0
            for envv in db_tabulate:
                VRGT[rgt][envv.upper()] = 0.0
            exec(banners)
            exec(actives)
            try:
                freagent = open('%s.out' % (rgt), 'r')
            except IOError:
                psi4.print_out('Warning: Output file \'%s.out\' not found.\n' % (rgt))
                psi4.print_out('         Database summary will have 0.0 and **** in its place.\n')
            else:
                while 1:
                    line = freagent.readline()
                    if not line:
                        if ERGT[rgt] == 0.0:
                            psi4.print_out('Warning: Output file \'%s.out\' has no DATABASE RESULT line.\n' % (rgt))
                            psi4.print_out('         Database summary will have 0.0 and **** in its place.\n')
                        break
                    s = line.split()
                    if (len(s) != 0) and (s[0:3] == ['DATABASE', 'RESULT:', 'computation']):
                        if int(s[3]) != db_linkage:
                            raise ValidationError('Output file \'%s.out\' has linkage %s incompatible with master.in linkage %s.'
                                % (rgt, str(s[3]), str(db_linkage)))
                        if s[6] != rgt:
                            raise ValidationError('Output file \'%s.out\' has nominal affiliation %s incompatible with reagent %s.'
                                % (rgt, s[6], rgt))
                        if (s[8:10] == ['electronic', 'energy']):
                            ERGT[rgt] = float(s[10])
                            psi4.print_out('DATABASE RESULT: electronic energy = %20.12f\n' % (ERGT[rgt]))
                        elif (s[8:10] == ['variable', 'value']):
                            for envv in db_tabulate:
                                envv = envv.upper()
                                if (s[13:] == envv.split()):
                                    VRGT[rgt][envv] = float(s[10])
                                    psi4.print_out('DATABASE RESULT: variable %s value    = %20.12f\n' % (envv, VRGT[rgt][envv]))
                freagent.close()

    #   end sow after writing files
    if db_mode == 'sow':
        return 0.0

    # Reap all the necessary reaction computations
    psi4.print_out("\n")
    p4util.banner(("Database %s Results" % (db_name)))
    psi4.print_out("\n")

    maxactv = []
    for rxn in HRXN:
        maxactv.append(len(ACTV[dbse + '-' + str(rxn)]))
    maxrgt = max(maxactv)
    table_delimit = '-' * (62 + 20 * maxrgt)
    tables = ''

    #   find any reactions that are incomplete
    FAIL = collections.defaultdict(int)
    for rxn in HRXN:
        db_rxn = dbse + '-' + str(rxn)
        for i in range(len(ACTV[db_rxn])):
            if abs(ERGT[ACTV[db_rxn][i]]) < 1.0e-12:
                FAIL[rxn] = 1

    #   tabulate requested process::environment variables
    tables += """   For each VARIABLE requested by tabulate, a 'Reaction Value' will be formed from\n"""
    tables += """   'Reagent' values according to weightings 'Wt', as for the REQUESTED ENERGY below.\n"""
    tables += """   Depending on the nature of the variable, this may or may not make any physical sense.\n"""
    for rxn in HRXN:
        db_rxn = dbse + '-' + str(rxn)
        VRXN[db_rxn] = {}

    for envv in db_tabulate:
        envv = envv.upper()
        tables += """\n   ==> %s <==\n\n""" % (envv.title())
        tables += tblhead(maxrgt, table_delimit, 2)

        for rxn in HRXN:
            db_rxn = dbse + '-' + str(rxn)

            if FAIL[rxn]:
                tables += """\n%23s   %8s %8s %8s %8s""" % (db_rxn, '', '****', '', '')
                for i in range(len(ACTV[db_rxn])):
                    tables += """ %16.8f %2.0f""" % (VRGT[ACTV[db_rxn][i]][envv], RXNM[db_rxn][ACTV[db_rxn][i]])

            else:
                VRXN[db_rxn][envv] = 0.0
                for i in range(len(ACTV[db_rxn])):
                    VRXN[db_rxn][envv] += VRGT[ACTV[db_rxn][i]][envv] * RXNM[db_rxn][ACTV[db_rxn][i]]

                tables += """\n%23s        %16.8f                  """ % (db_rxn, VRXN[db_rxn][envv])
                for i in range(len(ACTV[db_rxn])):
                    tables += """ %16.8f %2.0f""" % (VRGT[ACTV[db_rxn][i]][envv], RXNM[db_rxn][ACTV[db_rxn][i]])
        tables += """\n   %s\n""" % (table_delimit)

    #   tabulate primary requested energy variable with statistics
    count_rxn = 0
    minDerror = 100000.0
    maxDerror = 0.0
    MSDerror = 0.0
    MADerror = 0.0
    RMSDerror = 0.0

    tables += """\n   ==> %s <==\n\n""" % ('Requested Energy')
    tables += tblhead(maxrgt, table_delimit, 1)
    for rxn in HRXN:
        db_rxn = dbse + '-' + str(rxn)

        if FAIL[rxn]:
            tables += """\n%23s   %8.4f %8s %10s %10s""" % (db_rxn, BIND[db_rxn], '****', '****', '****')
            for i in range(len(ACTV[db_rxn])):
                tables += """ %16.8f %2.0f""" % (ERGT[ACTV[db_rxn][i]], RXNM[db_rxn][ACTV[db_rxn][i]])

        else:
            ERXN[db_rxn] = 0.0
            for i in range(len(ACTV[db_rxn])):
                ERXN[db_rxn] += ERGT[ACTV[db_rxn][i]] * RXNM[db_rxn][ACTV[db_rxn][i]]
            error = p4const.psi_hartree2kcalmol * ERXN[db_rxn] - BIND[db_rxn]

            tables += """\n%23s   %8.4f %8.4f %10.4f %10.4f""" % (db_rxn, BIND[db_rxn], p4const.psi_hartree2kcalmol * ERXN[db_rxn],
                error, error * p4const.psi_cal2J)
            for i in range(len(ACTV[db_rxn])):
                tables += """ %16.8f %2.0f""" % (ERGT[ACTV[db_rxn][i]], RXNM[db_rxn][ACTV[db_rxn][i]])

            if abs(error) < abs(minDerror):
                minDerror = error
            if abs(error) > abs(maxDerror):
                maxDerror = error
            MSDerror += error
            MADerror += abs(error)
            RMSDerror += error * error
            count_rxn += 1
    tables += """\n   %s\n""" % (table_delimit)

    if count_rxn:

        MSDerror /= float(count_rxn)
        MADerror /= float(count_rxn)
        RMSDerror = math.sqrt(RMSDerror / float(count_rxn))

        tables += """%23s %19s %10.4f %10.4f\n""" % ('Minimal Dev', '', minDerror, minDerror * p4const.psi_cal2J)
        tables += """%23s %19s %10.4f %10.4f\n""" % ('Maximal Dev', '', maxDerror, maxDerror * p4const.psi_cal2J)
        tables += """%23s %19s %10.4f %10.4f\n""" % ('Mean Signed Dev', '', MSDerror, MSDerror * p4const.psi_cal2J)
        tables += """%23s %19s %10.4f %10.4f\n""" % ('Mean Absolute Dev', '', MADerror, MADerror * p4const.psi_cal2J)
        tables += """%23s %19s %10.4f %10.4f\n""" % ('RMS Dev', '', RMSDerror, RMSDerror * p4const.psi_cal2J)
        tables += """   %s\n""" % (table_delimit)

        psi4.set_variable('%s DATABASE MEAN SIGNED DEVIATION' % (db_name), MSDerror)
        psi4.set_variable('%s DATABASE MEAN ABSOLUTE DEVIATION' % (db_name), MADerror)
        psi4.set_variable('%s DATABASE ROOT-MEAN-SQUARE DEVIATION' % (db_name), RMSDerror)

        psi4.print_out(tables)
        finalenergy = MADerror

    else:
        finalenergy = 0.0

    optstash.restore()

    DB_RGT.clear()
    DB_RGT.update(VRGT)
    DB_RXN.clear()
    DB_RXN.update(VRXN)
    return finalenergy


def tblhead(tbl_maxrgt, tbl_delimit, ttype):
    r"""Function that prints the header for the changable-width results tables in db().
    *tbl_maxrgt* is the number of reagent columns the table must plan for. *tbl_delimit*
    is a string of dashes of the correct length to set off the table. *ttype* is 1 for
    tables comparing the computed values to the reference or 2 for simple tabulation
    and sum of the computed values.

    """
    tbl_str = ''
    tbl_str += """   %s""" % (tbl_delimit)
    if ttype == 1:
        tbl_str += """\n%23s %19s %21s""" % ('Reaction', 'Reaction Energy', 'Reaction Error')
    elif ttype == 2:
        tbl_str += """\n%23s     %19s %17s""" % ('Reaction', 'Reaction Value', '')
    for i in range(tbl_maxrgt):
        tbl_str += """%20s""" % ('Reagent ' + str(i + 1))
    if ttype == 1:
        tbl_str += """\n%23s   %8s %8s %10s %10s""" % ('', 'Ref', 'Calc', '[kcal/mol]', '[kJ/mol]')
    elif ttype == 2:
        tbl_str += """\n%65s""" % ('')
    for i in range(tbl_maxrgt):
        if ttype == 1:
            tbl_str += """%20s""" % ('[Eh] Wt')
        elif ttype == 2:
            tbl_str += """%20s""" % ('Value Wt')
    tbl_str += """\n   %s""" % (tbl_delimit)
    return tbl_str

##  Aliases  ##
db = database

#######################
##  End of Database  ##
#######################


###################################
##  Start of Complete Basis Set  ##
###################################

def complete_basis_set(func=energy, **kwargs):
    r"""Function to define a multistage energy method from combinations of
    basis set extrapolations and delta corrections and condense the
    components into a minimum number of calculations.

    :aliases: cbs()

    :returns: (*float*) -- Total electronic energy in Hartrees

    :PSI variables:

    .. hlist::
       :columns: 1

       * :psivar:`CBS TOTAL ENERGY <CBSTOTALENERGY>`
       * :psivar:`CBS REFERENCE ENERGY <CBSREFERENCEENERGY>`
       * :psivar:`CBS CORRELATION ENERGY <CBSCORRELATIONENERGY>`
       * :psivar:`CURRENT ENERGY <CURRENTENERGY>`
       * :psivar:`CURRENT REFERENCE ENERGY <CURRENTREFERENCEENERGY>`
       * :psivar:`CURRENT CORRELATION ENERGY <CURRENTCORRELATIONENERGY>`

    .. caution:: Some features are not yet implemented. Buy a developer a coffee.

       - No way to tell function to boost fitting basis size for all calculations.

       - No way to extrapolate def2 family basis sets

       - Need to add more extrapolation schemes

    As represented in the equation below, a CBS energy method is defined in several
    sequential stages (scf, corl, delta, delta2, delta3, delta4, delta5) covering treatment
    of the reference total energy, the correlation energy, a delta correction to the
    correlation energy, and a second delta correction, etc.. Each is activated by its
    stage_wfn keyword and is only allowed if all preceding stages are active.

    .. include:: cbs_eqn.rst

    * Energy Methods
        The presence of a stage_wfn keyword is the indicator to incorporate
        (and check for stage_basis and stage_scheme keywords) and compute
        that stage in defining the CBS energy.

        The cbs() function requires, at a minimum, ``name='scf'`` and ``scf_basis``
        keywords to be specified for reference-step only jobs and ``name`` and
        ``corl_basis`` keywords for correlated jobs.

        The following energy methods have been set up for cbs().

        .. hlist::
           :columns: 5

           * scf
           * mp2
           * mp2.5
           * mp3
           * mp4(sdq)
           * mp4
           * mp\ *n*
           * omp2
           * omp2.5
           * omp3
           * olccd
           * lccd
           * lccsd
           * cepa(0)
           * cepa(1)
           * cepa(3)
           * acpf
           * aqcc
           * qcisd
           * cc2
           * ccsd
           * fno-ccsd
           * bccd
           * cc3
           * qcisd(t)
           * ccsd(t)
           * fno-ccsd(t)
           * bccd(t)
           * cisd
           * cisdt
           * cisdtq
           * ci\ *n*
           * fci
           * mrccsd
           * mrccsd(t)
           * mrccsdt
           * mrccsdt(q)

    :type name: string
    :param name: ``'scf'`` || ``'ccsd'`` || etc.

        First argument, usually unlabeled. Indicates the computational method
        for the correlation energy, unless only reference step to be performed,
        in which case should be ``'scf'``. Overruled if stage_wfn keywords supplied.

    :type scf_wfn: string
    :param scf_wfn: |dl| ``'scf'`` |dr| || ``'c4-scf'`` || etc.

        Indicates the energy method for which the reference energy is to be
        obtained. Generally unnecessary, as 'scf' is *the* scf in |PSIfour| but
        can be used to direct lone scf components to run in |PSIfour| or Cfour
        in a mixed-program composite method.

    :type corl_wfn: string
    :param corl_wfn: ``'mp2'`` || ``'ccsd(t)'`` || etc.

        Indicates the energy method for which the correlation energy is to be
        obtained. Can also be specified with ``name`` or as the unlabeled
        first argument to the function.

    :type delta_wfn: string
    :param delta_wfn: ``'ccsd'`` || ``'ccsd(t)'`` || etc.

        Indicates the (superior) energy method for which a delta correction
        to the correlation energy is to be obtained.

    :type delta_wfn_lesser: string
    :param delta_wfn_lesser: |dl| ``corl_wfn`` |dr| || ``'mp2'`` || etc.

        Indicates the inferior energy method for which a delta correction
        to the correlation energy is to be obtained.

    :type delta2_wfn: string
    :param delta2_wfn: ``'ccsd'`` || ``'ccsd(t)'`` || etc.

        Indicates the (superior) energy method for which a second delta correction
        to the correlation energy is to be obtained.

    :type delta2_wfn_lesser: string
    :param delta2_wfn_lesser: |dl| ``delta_wfn`` |dr| || ``'ccsd(t)'`` || etc.

        Indicates the inferior energy method for which a second delta correction
        to the correlation energy is to be obtained.

    :type delta3_wfn: string
    :param delta3_wfn: ``'ccsd'`` || ``'ccsd(t)'`` || etc.

        Indicates the (superior) energy method for which a third delta correction
        to the correlation energy is to be obtained.

    :type delta3_wfn_lesser: string
    :param delta3_wfn_lesser: |dl| ``delta2_wfn`` |dr| || ``'ccsd(t)'`` || etc.

        Indicates the inferior energy method for which a third delta correction
        to the correlation energy is to be obtained.

    :type delta4_wfn: string
    :param delta4_wfn: ``'ccsd'`` || ``'ccsd(t)'`` || etc.

        Indicates the (superior) energy method for which a fourth delta correction
        to the correlation energy is to be obtained.

    :type delta4_wfn_lesser: string
    :param delta4_wfn_lesser: |dl| ``delta3_wfn`` |dr| || ``'ccsd(t)'`` || etc.

        Indicates the inferior energy method for which a fourth delta correction
        to the correlation energy is to be obtained.

    :type delta5_wfn: string
    :param delta5_wfn: ``'ccsd'`` || ``'ccsd(t)'`` || etc.

        Indicates the (superior) energy method for which a fifth delta correction
        to the correlation energy is to be obtained.

    :type delta5_wfn_lesser: string
    :param delta5_wfn_lesser: |dl| ``delta4_wfn`` |dr| || ``'ccsd(t)'`` || etc.

        Indicates the inferior energy method for which a fifth delta correction
        to the correlation energy is to be obtained.

    * Basis Sets
        Currently, the basis set set through ``set`` commands have no influence
        on a cbs calculation.

    :type scf_basis: :ref:`basis string <apdx:basisElement>`
    :param scf_basis: |dl| ``corl_basis`` |dr| || ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the reference energy.
        If any correlation method is specified, ``scf_basis`` can default
        to ``corl_basis``.

    :type corl_basis: :ref:`basis string <apdx:basisElement>`
    :param corl_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the correlation energy.

    :type delta_basis: :ref:`basis string <apdx:basisElement>`
    :param delta_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the delta correction
        to the correlation energy.

    :type delta2_basis: :ref:`basis string <apdx:basisElement>`
    :param delta2_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the second delta correction
        to the correlation energy.

    :type delta3_basis: :ref:`basis string <apdx:basisElement>`
    :param delta3_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the third delta correction
        to the correlation energy.

    :type delta4_basis: :ref:`basis string <apdx:basisElement>`
    :param delta4_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the fourth delta correction
        to the correlation energy.

    :type delta5_basis: :ref:`basis string <apdx:basisElement>`
    :param delta5_basis: ``'cc-pV[TQ]Z'`` || ``'jun-cc-pv[tq5]z'`` || ``'6-31G*'`` || etc.

        Indicates the sequence of basis sets employed for the fifth delta correction
        to the correlation energy.

    * Schemes
        Transformations of the energy through basis set extrapolation for each
        stage of the CBS definition. A complaint is generated if number of basis
        sets in stage_basis does not exactly satisfy requirements of stage_scheme.
        An exception is the default, ``'xtpl_highest_1'``, which uses the best basis
        set available. See `Extrapolation Schemes`_ for all available schemes.

    :type scf_scheme: function
    :param scf_scheme: |dl| ``xtpl_highest_1`` |dr| || ``scf_xtpl_helgaker_3`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the reference energy.
        Defaults to :py:func:`~driver_util.scf_xtpl_helgaker_3` if three valid basis sets
        present in ``scf_basis``, :py:func:`~driver_util.scf_xtpl_helgaker_2` if two valid basis
        sets present in ``scf_basis``, and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type corl_scheme: function
    :param corl_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``corl_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type delta_scheme: function
    :param delta_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the delta correction
        to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``delta_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type delta2_scheme: function
    :param delta2_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the second delta correction
        to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``delta2_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type delta3_scheme: function
    :param delta3_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the third delta correction
        to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``delta3_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type delta4_scheme: function
    :param delta4_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the fourth delta correction
        to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``delta4_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type delta5_scheme: function
    :param delta5_scheme: |dl| ``xtpl_highest_1`` |dr| || ``corl_xtpl_helgaker_2`` || etc.

        Indicates the basis set extrapolation scheme to be applied to the fifth delta correction
        to the correlation energy.
        Defaults to :py:func:`~driver_util.corl_xtpl_helgaker_2` if two valid basis sets
        present in ``delta5_basis`` and :py:func:`~driver_util.xtpl_highest_1` otherwise.

    :type molecule: :ref:`molecule <op_py_molecule>`
    :param molecule: ``h2o`` || etc.

        The target molecule, if not the last molecule defined.

    :examples:


    >>> # [1] replicates with cbs() the simple model chemistry scf/cc-pVDZ: set basis cc-pVDZ energy('scf')
    >>> cbs(name='scf', scf_basis='cc-pVDZ')

    >>> # [2] replicates with cbs() the simple model chemistry mp2/jun-cc-pVDZ: set basis jun-cc-pVDZ energy('mp2')
    >>> cbs(name='mp2', corl_basis='jun-cc-pVDZ')

    >>> # [3] DTQ-zeta extrapolated scf reference energy
    >>> cbs(name='scf', scf_basis='cc-pV[DTQ]Z', scf_scheme=scf_xtpl_helgaker_3)

    >>> # [4] DT-zeta extrapolated mp2 correlation energy atop a T-zeta reference
    >>> cbs(corl_wfn='mp2', corl_basis='cc-pv[dt]z', corl_scheme=corl_xtpl_helgaker_2)

    >>> # [5] a DT-zeta extrapolated coupled-cluster correction atop a TQ-zeta extrapolated mp2 correlation energy atop a Q-zeta reference (both equivalent)
    >>> cbs(corl_wfn='mp2', corl_basis='aug-cc-pv[tq]z', delta_wfn='ccsd(t)', delta_basis='aug-cc-pv[dt]z')
    >>> cbs(energy, wfn='mp2', corl_basis='aug-cc-pv[tq]z', corl_scheme=corl_xtpl_helgaker_2, delta_wfn='ccsd(t)', delta_basis='aug-cc-pv[dt]z', delta_scheme=corl_xtpl_helgaker_2)

    >>> # [6] a D-zeta ccsd(t) correction atop a DT-zeta extrapolated ccsd cluster correction atop a TQ-zeta extrapolated mp2 correlation energy atop a Q-zeta reference
    >>> cbs(name='mp2', corl_basis='aug-cc-pv[tq]z', corl_scheme=corl_xtpl_helgaker_2, delta_wfn='ccsd', delta_basis='aug-cc-pv[dt]z', delta_scheme=corl_xtpl_helgaker_2, delta2_wfn='ccsd(t)', delta2_wfn_lesser='ccsd', delta2_basis='aug-cc-pvdz')

    >>> # [7] cbs() coupled with database()
    >>> TODO database('mp2', 'BASIC', subset=['h2o','nh3'], symm='on', func=cbs, corl_basis='cc-pV[tq]z', corl_scheme=corl_xtpl_helgaker_2, delta_wfn='ccsd(t)', delta_basis='sto-3g')

    >>> # [8] cbs() coupled with optimize()
    >>> TODO optimize('mp2', corl_basis='cc-pV[DT]Z', corl_scheme=corl_xtpl_helgaker_2, func=cbs)

    """
    kwargs = p4util.kwargs_lower(kwargs)
    return_wfn = kwargs.pop('return_wfn', False)

    # Establish function to call (only energy makes sense for cbs)
    if func not in [energy, gradient, hessian]:
        raise ValidationError("""Wrapper complete_basis_set is unhappy to be calling function '%s' instead of 'energy'.""" % (func.__name__))

    optstash = p4util.OptionsState(
        ['BASIS'],
        ['WFN'],
        ['WRITER_FILE_LABEL'])

    # Define some quantum chemical knowledge, namely what methods are subsumed in others

    do_scf = True
    do_corl = False
    do_delta = False
    do_delta2 = False
    do_delta3 = False
    do_delta4 = False
    do_delta5 = False

    user_writer_file_label = psi4.get_global_option('WRITER_FILE_LABEL')

    # Make sure the molecule the user provided is the active one
    molecule = kwargs.pop('molecule', psi4.get_active_molecule())
    molecule.update_geometry()
    molstr = molecule.create_psi4_string_from_molecule()
    natom = molecule.natom()

    # Establish method for reference energy
    if 'scf_wfn' in kwargs:
        cbs_scf_wfn = kwargs['scf_wfn'].lower()
    elif 'name' in kwargs and (kwargs['name'].lower() in ['hf', 'scf', 'c4-scf']):
        cbs_scf_wfn = kwargs['name'].lower()
    else:
        cbs_scf_wfn = 'hf'

    if do_scf:
        if cbs_scf_wfn not in VARH.keys():
            raise ValidationError("""Requested SCF method '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_scf_wfn))

    # Establish method for correlation energy
    if 'corl_wfn' in kwargs:
        do_corl = True
        cbs_corl_wfn = kwargs['corl_wfn'].lower()
    elif 'name' in kwargs and (kwargs['name'].lower() not in ['hf', 'scf', 'c4-scf']):
        do_corl = True
        cbs_corl_wfn = kwargs['name'].lower()
    else:
        pass

    if do_corl:
        if cbs_corl_wfn not in VARH.keys():
            raise ValidationError("""Requested CORL method '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_corl_wfn))

        cbs_corl_wfn_lesser = kwargs.get('corl_wfn_lesser', cbs_scf_wfn).lower()
        if cbs_corl_wfn_lesser not in VARH.keys():
            raise ValidationError("""Requested CORL method lesser '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta_wfn_lesser))

    # Establish method for delta correction energy
    if 'delta_wfn' in kwargs:
        do_delta = True
        cbs_delta_wfn = kwargs['delta_wfn'].lower()
        if cbs_delta_wfn not in VARH.keys():
            raise ValidationError("""Requested DELTA method '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta_wfn))

        cbs_delta_wfn_lesser = kwargs.get('delta_wfn_lesser', cbs_corl_wfn).lower()
        if cbs_delta_wfn_lesser not in VARH.keys():
            raise ValidationError("""Requested DELTA method lesser '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta_wfn_lesser))

    # Establish method for second delta correction energy
    if 'delta2_wfn' in kwargs:
        do_delta2 = True
        cbs_delta2_wfn = kwargs['delta2_wfn'].lower()
        if cbs_delta2_wfn not in VARH.keys():
            raise ValidationError("""Requested DELTA2 method '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta2_wfn))

        cbs_delta2_wfn_lesser = kwargs.get('delta2_wfn_lesser', cbs_delta_wfn).lower()
        if cbs_delta2_wfn_lesser not in VARH.keys():
            raise ValidationError("""Requested DELTA2 method lesser '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta2_wfn_lesser))

#    # Establish method for third delta correction energy
#    if 'delta3_wfn' in kwargs:
#        do_delta3 = True
#        cbs_delta3_wfn = kwargs['delta3_wfn'].lower()
#        if cbs_delta3_wfn not in VARH.keys():
#            raise ValidationError("""Requested DELTA3 method '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta3_wfn))
#
#        cbs_delta3_wfn_lesser = kwargs.get('delta3_wfn_lesser', cbs_delta2_wfn).lower()
#        if not (cbs_delta3_wfn_lesser in VARH.keys()):
#            raise ValidationError("""Requested DELTA3 method lesser '%s' is not recognized. Add it to VARH in wrapper.py to proceed.""" % (cbs_delta3_wfn_lesser))
#
#    # Establish method for fourth delta correction energy
#    if 'delta4_wfn' in kwargs:
#        do_delta4 = True
#        cbs_delta4_wfn = kwargs['delta4_wfn'].lower()
#        if not (cbs_delta4_wfn in VARH.keys()):
#            raise ValidationError('Requested DELTA4 method \'%s\' is not recognized. Add it to VARH in wrapper.py to proceed.' % (cbs_delta4_wfn))
#
#        if 'delta4_wfn_lesser' in kwargs:
#            cbs_delta4_wfn_lesser = kwargs['delta4_wfn_lesser'].lower()
#        else:
#            cbs_delta4_wfn_lesser = cbs_delta3_wfn
#        if not (cbs_delta4_wfn_lesser in VARH.keys()):
#            raise ValidationError('Requested DELTA4 method lesser \'%s\' is not recognized. Add it to VARH in wrapper.py to proceed.' % (cbs_delta4_wfn_lesser))
#
#    # Establish method for fifth delta correction energy
#    if 'delta5_wfn' in kwargs:
#        do_delta5 = True
#        cbs_delta5_wfn = kwargs['delta5_wfn'].lower()
#        if not (cbs_delta5_wfn in VARH.keys()):
#            raise ValidationError('Requested DELTA5 method \'%s\' is not recognized. Add it to VARH in wrapper.py to proceed.' % (cbs_delta5_wfn))
#
#        if 'delta5_wfn_lesser' in kwargs:
#            cbs_delta5_wfn_lesser = kwargs['delta5_wfn_lesser'].lower()
#        else:
#            cbs_delta5_wfn_lesser = cbs_delta4_wfn
#        if not (cbs_delta5_wfn_lesser in VARH.keys()):
#            raise ValidationError('Requested DELTA5 method lesser \'%s\' is not recognized. Add it to VARH in wrapper.py to proceed.' % (cbs_delta5_wfn_lesser))

    # Check that user isn't skipping steps in scf + corl + delta + delta2 sequence
    if   do_scf and not do_corl and not do_delta and not do_delta2 and not do_delta3 and not do_delta4 and not do_delta5:
        pass
    elif do_scf and do_corl and not do_delta and not do_delta2 and not do_delta3 and not do_delta4 and not do_delta5:
        pass
    elif do_scf and do_corl and do_delta and not do_delta2 and not do_delta3 and not do_delta4 and not do_delta5:
        pass
    elif do_scf and do_corl and do_delta and do_delta2 and not do_delta3 and not do_delta4 and not do_delta5:
        pass
    #elif do_scf and do_corl and do_delta and do_delta2 and do_delta3 and not do_delta4 and not do_delta5:
    #    pass
    #elif do_scf and do_corl and do_delta and do_delta2 and do_delta3 and do_delta4 and not do_delta5:
    #    pass
    #elif do_scf and do_corl and do_delta and do_delta2 and do_delta3 and do_delta4 and do_delta5:
    #    pass
    else:
        raise ValidationError('Requested scf (%s) + corl (%s) + delta (%s) + delta2 (%s) + delta3 (%s) + delta4 (%s) + delta5 (%s) not valid. These steps are cummulative.' %
            (do_scf, do_corl, do_delta, do_delta2, do_delta3, do_delta4, do_delta5))

    # Establish list of valid basis sets for correlation energy
    if do_corl:
        if 'corl_basis' in kwargs:
            BSTC, ZETC = driver_util.expand_bracketed_basis(kwargs['corl_basis'].lower(), molecule=molstr)
        else:
            raise ValidationError("""CORL basis sets through keyword '%s' are required.""" % ('corl_basis'))

    # Establish list of valid basis sets for scf energy
    if 'scf_basis' in kwargs:
        BSTR, ZETR = driver_util.expand_bracketed_basis(kwargs['scf_basis'].lower(), molecule=molstr)
    elif do_corl:
        BSTR = BSTC[:]
        ZETR = ZETC[:]
    else:
        raise ValidationError("""SCF basis sets through keyword '%s' are required. Or perhaps you forgot the '%s'.""" % ('scf_basis', 'corl_wfn'))

    # Establish list of valid basis sets for delta correction energy
    if do_delta:
        if 'delta_basis' in kwargs:
            BSTD, ZETD = driver_util.expand_bracketed_basis(kwargs['delta_basis'].lower(), molecule=molstr)
        else:
            raise ValidationError("""DELTA basis sets through keyword '%s' are required.""" % ('delta_basis'))

    # Establish list of valid basis sets for second delta correction energy
    if do_delta2:
        if 'delta2_basis' in kwargs:
            BSTD2, ZETD2 = driver_util.expand_bracketed_basis(kwargs['delta2_basis'].lower(), molecule=molstr)
        else:
            raise ValidationError("""DELTA2 basis sets through keyword '%s' are required.""" % ('delta2_basis'))

#    # Establish list of valid basis sets for third delta correction energy
#    if do_delta3:
#        if 'delta3_basis' in kwargs:
#            BSTD3, ZETD3 = validate_bracketed_basis(kwargs['delta3_basis'].lower())
#        else:
#            raise ValidationError('DELTA3 basis sets through keyword \'%s\' are required.' % ('delta3_basis'))
#
#    # Establish list of valid basis sets for fourth delta correction energy
#    if do_delta4:
#        if 'delta4_basis' in kwargs:
#            BSTD4, ZETD4 = validate_bracketed_basis(kwargs['delta4_basis'].lower())
#        else:
#            raise ValidationError('DELTA4 basis sets through keyword \'%s\' are required.' % ('delta4_basis'))
#
#    # Establish list of valid basis sets for fifth delta correction energy
#    if do_delta5:
#        if 'delta5_basis' in kwargs:
#            BSTD5, ZETD5 = validate_bracketed_basis(kwargs['delta5_basis'].lower())
#        else:
#            raise ValidationError('DELTA5 basis sets through keyword \'%s\' are required.' % ('delta5_basis'))

    # Establish treatment for scf energy (validity check useless since python will catch it long before here)
    if (len(BSTR) == 3) and ('scf_basis' in kwargs):
        cbs_scf_scheme = driver_util.scf_xtpl_helgaker_3
    elif (len(BSTR) == 2) and ('scf_basis' in kwargs):
        cbs_scf_scheme = driver_util.scf_xtpl_helgaker_2
    else:
        cbs_scf_scheme = driver_util.xtpl_highest_1
    if 'scf_scheme' in kwargs:
        cbs_scf_scheme = kwargs['scf_scheme']

    # Establish treatment for correlation energy
    if do_corl:
        if len(BSTC) == 2:
            cbs_corl_scheme = driver_util.corl_xtpl_helgaker_2
        else:
            cbs_corl_scheme = driver_util.xtpl_highest_1
        if 'corl_scheme' in kwargs:
            cbs_corl_scheme = kwargs['corl_scheme']

    # Establish treatment for delta correction energy
    if do_delta:
        if len(BSTD) == 2:
            cbs_delta_scheme = driver_util.corl_xtpl_helgaker_2
        else:
            cbs_delta_scheme = driver_util.xtpl_highest_1
        if 'delta_scheme' in kwargs:
            cbs_delta_scheme = kwargs['delta_scheme']

    # Establish treatment for delta2 correction energy
    if do_delta2:
        if len(BSTD2) == 2:
            cbs_delta2_scheme = driver_util.corl_xtpl_helgaker_2
        else:
            cbs_delta2_scheme = driver_util.xtpl_highest_1
        if 'delta2_scheme' in kwargs:
            cbs_delta2_scheme = kwargs['delta2_scheme']

#    # Establish treatment for delta3 correction energy
#    if do_delta3:
#        if len(BSTD3) == 2:
#            cbs_delta3_scheme = corl_xtpl_helgaker_2
#        else:
#            cbs_delta3_scheme = xtpl_highest_1
#        if 'delta3_scheme' in kwargs:
#            cbs_delta3_scheme = kwargs['delta3_scheme']
#
#    # Establish treatment for delta4 correction energy
#    if do_delta4:
#        if len(BSTD4) == 2:
#            cbs_delta4_scheme = corl_xtpl_helgaker_2
#        else:
#            cbs_delta4_scheme = xtpl_highest_1
#        if 'delta4_scheme' in kwargs:
#            cbs_delta4_scheme = kwargs['delta4_scheme']
#
#    # Establish treatment for delta5 correction energy
#    if do_delta5:
#        if len(BSTD5) == 2:
#            cbs_delta5_scheme = corl_xtpl_helgaker_2
#        else:
#            cbs_delta5_scheme = xtpl_highest_1
#        if 'delta5_scheme' in kwargs:
#            cbs_delta5_scheme = kwargs['delta5_scheme']

    # Build string of title banner
    cbsbanners = ''
    cbsbanners += """psi4.print_out('\\n')\n"""
    cbsbanners += """p4util.banner(' CBS Setup ')\n"""
    cbsbanners += """psi4.print_out('\\n')\n\n"""
    exec(cbsbanners)

    # Call schemes for each portion of total energy to 'place orders' for calculations needed
    d_fields = ['d_stage', 'd_scheme', 'd_basis', 'd_wfn', 'd_need', 'd_coef', 'd_energy', 'd_gradient', 'd_hessian']
    f_fields = ['f_wfn', 'f_basis', 'f_zeta', 'f_energy', 'f_gradient', 'f_hessian']
    GRAND_NEED = []
    MODELCHEM = []
    if do_scf:
        NEED = expand_scheme_orders(cbs_scf_scheme, BSTR, ZETR, cbs_scf_wfn, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['scf', cbs_scf_scheme,
            driver_util.contract_bracketed_basis(BSTR, True if cbs_scf_scheme == driver_util.xtpl_highest_1 else False),
            cbs_scf_wfn, NEED, +1, 0.0, None, None])))

    if do_corl:
        NEED = expand_scheme_orders(cbs_corl_scheme, BSTC, ZETC, cbs_corl_wfn, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['corl', cbs_corl_scheme,
            driver_util.contract_bracketed_basis(BSTC, True if cbs_corl_scheme == driver_util.xtpl_highest_1 else False),
            cbs_corl_wfn, NEED, +1, 0.0, None, None])))

        NEED = expand_scheme_orders(cbs_corl_scheme, BSTC, ZETC, cbs_corl_wfn_lesser, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['corl', cbs_corl_scheme,
            driver_util.contract_bracketed_basis(BSTC, True if cbs_corl_scheme == driver_util.xtpl_highest_1 else False),
            cbs_corl_wfn_lesser, NEED, -1, 0.0, None, None])))

    if do_delta:
        NEED = expand_scheme_orders(cbs_delta_scheme, BSTD, ZETD, cbs_delta_wfn, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['delta', cbs_delta_scheme,
            driver_util.contract_bracketed_basis(BSTD, True if cbs_delta_scheme == driver_util.xtpl_highest_1 else False),
            cbs_delta_wfn, NEED, +1, 0.0, None, None])))

        NEED = expand_scheme_orders(cbs_delta_scheme, BSTD, ZETD, cbs_delta_wfn_lesser, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['delta', cbs_delta_scheme,
            driver_util.contract_bracketed_basis(BSTD, True if cbs_delta_scheme == driver_util.xtpl_highest_1 else False),
            cbs_delta_wfn_lesser, NEED, -1, 0.0, None, None])))

    if do_delta2:
        NEED = expand_scheme_orders(cbs_delta2_scheme, BSTD2, ZETD2, cbs_delta2_wfn, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['delta2', cbs_delta2_scheme,
            driver_util.contract_bracketed_basis(BSTD2, True if cbs_delta2_scheme == driver_util.xtpl_highest_1 else False),
            cbs_delta2_wfn, NEED, +1, 0.0, None, None])))

        NEED = expand_scheme_orders(cbs_delta2_scheme, BSTD2, ZETD2, cbs_delta2_wfn_lesser, natom)
        GRAND_NEED.append(dict(zip(d_fields, ['delta2', cbs_delta2_scheme,
            driver_util.contract_bracketed_basis(BSTD2, True if cbs_delta2_scheme == driver_util.xtpl_highest_1 else False),
            cbs_delta2_wfn_lesser, NEED, -1, 0.0, None, None])))

#    if do_delta3:
#        NEED = call_function_in_1st_argument(cbs_delta3_scheme,
#            mode='requisition', basisname=BSTD3, basiszeta=ZETD3, wfnname=cbs_delta3_wfn)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta3', cbs_delta3_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta3_wfn, NEED, +1, 0.0])))
#
#        NEED = call_function_in_1st_argument(cbs_delta3_scheme,
#            mode='requisition', basisname=BSTD3, basiszeta=ZETD3, wfnname=cbs_delta3_wfn_lesser)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta3', cbs_delta3_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta3_wfn_lesser, NEED, -1, 0.0])))
#
#    if do_delta4:
#        NEED = call_function_in_1st_argument(cbs_delta4_scheme,
#            mode='requisition', basisname=BSTD4, basiszeta=ZETD4, wfnname=cbs_delta4_wfn)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta4', cbs_delta4_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta4_wfn, NEED, +1, 0.0])))
#
#        NEED = call_function_in_1st_argument(cbs_delta4_scheme,
#            mode='requisition', basisname=BSTD4, basiszeta=ZETD4, wfnname=cbs_delta4_wfn_lesser)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta4', cbs_delta4_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta4_wfn_lesser, NEED, -1, 0.0])))
#
#    if do_delta5:
#        NEED = call_function_in_1st_argument(cbs_delta5_scheme,
#            mode='requisition', basisname=BSTD5, basiszeta=ZETD5, wfnname=cbs_delta5_wfn)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta5', cbs_delta5_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta5_wfn, NEED, +1, 0.0])))
#
#        NEED = call_function_in_1st_argument(cbs_delta5_scheme,
#            mode='requisition', basisname=BSTD5, basiszeta=ZETD5, wfnname=cbs_delta5_wfn_lesser)
#        GRAND_NEED.append(dict(zip(d_fields, ['delta5', cbs_delta5_scheme,
#            reconstitute_bracketed_basis(NEED), cbs_delta5_wfn_lesser, NEED, -1, 0.0])))

    for stage in GRAND_NEED:
        for lvl in stage['d_need'].items():
            MODELCHEM.append(lvl[1])

    # Apply chemical reasoning to choose the minimum computations to run
    JOBS = MODELCHEM[:]

    addlremark = {energy: '', gradient: ', GRADIENT', hessian: ', HESSIAN'}
    instructions = ''
    instructions += """    Naive listing of computations required.\n"""
    for mc in JOBS:
        instructions += """   %12s / %-24s for  %s%s\n""" % \
            (mc['f_wfn'], mc['f_basis'], VARH[mc['f_wfn']][mc['f_wfn']], addlremark[func])

    #     Remove duplicate modelchem portion listings
    for mc in MODELCHEM:
        dups = -1
        for indx_job, job in enumerate(JOBS):
            if (job['f_wfn'] == mc['f_wfn']) and (job['f_basis'] == mc['f_basis']):
                dups += 1
                if dups >= 1:
                    del JOBS[indx_job]

    #     Remove chemically subsumed modelchem portion listings
    if func == energy:
        for mc in MODELCHEM:
            for wfn in VARH[mc['f_wfn']]:
                for indx_job, job in enumerate(JOBS):
                    if (VARH[mc['f_wfn']][wfn] == VARH[job['f_wfn']][job['f_wfn']]) and \
                       (mc['f_basis'] == job['f_basis']) and not \
                       (mc['f_wfn'] == job['f_wfn']):
                        del JOBS[indx_job]

    instructions += """\n    Enlightened listing of computations required.\n"""
    for mc in JOBS:
        instructions += """   %12s / %-24s for  %s%s\n""" % \
            (mc['f_wfn'], mc['f_basis'], VARH[mc['f_wfn']][mc['f_wfn']], addlremark[func])

    #     Expand listings to all that will be obtained
    JOBS_EXT = []
    for job in JOBS:
        for wfn in VARH[job['f_wfn']]:
            JOBS_EXT.append(dict(zip(f_fields, [wfn, job['f_basis'], job['f_zeta'],
                                                0.0,
                                                psi4.Matrix(natom, 3),
                                                psi4.Matrix(3 * natom, 3 * natom)])))

    instructions += """\n    Full listing of computations to be obtained (required and bonus).\n"""
    for mc in JOBS_EXT:
        instructions += """   %12s / %-24s for  %s%s\n""" % \
            (mc['f_wfn'], mc['f_basis'], VARH[mc['f_wfn']][mc['f_wfn']], addlremark[func])
    psi4.print_out(instructions)

    psioh = psi4.IOManager.shared_object()
    psioh.set_specific_retention(p4const.PSIF_SCF_MOS, True)
    # projection across point groups not allowed and cbs() usually a mix of symm-enabled and symm-tol calls
    #   needs to be communicated to optimize() so reset by that optstash
    psi4.set_local_option('SCF', 'GUESS_PERSIST', True)

    Njobs = 0
    # Run necessary computations
    for mc in JOBS:
        kwargs['name'] = mc['f_wfn']

        # Build string of title banner
        cbsbanners = ''
        cbsbanners += """psi4.print_out('\\n')\n"""
        cbsbanners += """p4util.banner(' CBS Computation: %s / %s%s ')\n""" % \
            (mc['f_wfn'].upper(), mc['f_basis'].upper(), addlremark[func])
        cbsbanners += """psi4.print_out('\\n')\n\n"""
        exec(cbsbanners)

        # Build string of molecule and commands that are dependent on the database
        commands = '\n'
        commands += """\npsi4.set_global_option('BASIS', '%s')\n""" % (mc['f_basis'])
        commands += """psi4.set_global_option('WRITER_FILE_LABEL', '%s')\n""" % \
            (user_writer_file_label + ('' if user_writer_file_label == '' else '-') + mc['f_wfn'].lower() + '-' + mc['f_basis'].lower())
        exec(commands)

        # Make energy(), etc. call
        response = func(molecule=molecule, **kwargs)
        if func == energy:
            mc['f_energy'] = response
        elif func == gradient:
            mc['f_gradient'] = response
            mc['f_energy'] = psi4.get_variable('CURRENT ENERGY')
        elif func == hessian:
            mc['f_hessian'] = response
            mc['f_energy'] = psi4.get_variable('CURRENT ENERGY')
        Njobs += 1
        psi4.print_out(str(mc['f_energy']))
        mc['f_gradient'].print_out()
        mc['f_hessian'].print_out()

        # Fill in energies for subsumed methods
        if func == energy:
            for wfn in VARH[mc['f_wfn']]:
                for job in JOBS_EXT:
                    if (wfn == job['f_wfn']) and (mc['f_basis'] == job['f_basis']):
                        job['f_energy'] = psi4.get_variable(VARH[wfn][wfn])

        psi4.print_variables()
        psi4.clean_variables()
        psi4.clean()

        # Copy data from 'run' to 'obtained' table
        for mce in JOBS_EXT:
            if (mc['f_wfn'] == mce['f_wfn']) and (mc['f_basis'] == mce['f_basis']):
                mce['f_energy'] = mc['f_energy']
                mce['f_gradient'] = mc['f_gradient']
                mce['f_hessian'] = mc['f_hessian']

    psioh.set_specific_retention(p4const.PSIF_SCF_MOS, False)

    # Build string of title banner
    cbsbanners = ''
    cbsbanners += """psi4.print_out('\\n')\n"""
    cbsbanners += """p4util.banner(' CBS Results ')\n"""
    cbsbanners += """psi4.print_out('\\n')\n\n"""
    exec(cbsbanners)

    # Insert obtained energies into the array that stores the cbs stages
    for stage in GRAND_NEED:
        for lvl in stage['d_need'].items():
            MODELCHEM.append(lvl[1])

            for job in JOBS_EXT:
                if (((lvl[1]['f_wfn'] == job['f_wfn']) or
                     ((lvl[1]['f_wfn'][3:] == job['f_wfn']) and lvl[1]['f_wfn'].startswith('c4-')) or
                     ((lvl[1]['f_wfn'] == job['f_wfn'][3:]) and job['f_wfn'].startswith('c4-')) or
                     (('c4-' + lvl[1]['f_wfn']) == job['f_wfn']) or
                     (lvl[1]['f_wfn'] == ('c4-' + job['f_wfn']))) and
                     (lvl[1]['f_basis'] == job['f_basis'])):
                    lvl[1]['f_energy'] = job['f_energy']
                    lvl[1]['f_gradient'] = job['f_gradient']
                    lvl[1]['f_hessian'] = job['f_hessian']

    # Make xtpl() call
    finalenergy = 0.0
    finalgradient = psi4.Matrix(natom, 3)
    finalhessian = psi4.Matrix(3 * natom, 3 * natom)
    for stage in GRAND_NEED:
        hiloargs = contract_scheme_orders(stage['d_need'], 'f_energy')
        stage['d_energy'] = stage['d_scheme'](**hiloargs)
        finalenergy += stage['d_energy'] * stage['d_coef']

        if func == gradient:
            hiloargs = contract_scheme_orders(stage['d_need'], 'f_gradient')
            stage['d_gradient'] = stage['d_scheme'](**hiloargs)
            work = stage['d_gradient'].clone()
            work.scale(stage['d_coef'])
            finalgradient.add(work)

        elif func == hessian:
            hiloargs = contract_scheme_orders(stage['d_need'], 'f_hessian')
            stage['d_hessian'] = stage['d_scheme'](**hiloargs)
            work = stage['d_hessian'].clone()
            work.scale(stage['d_coef'])
            finalhessian.add(work)

    # Build string of results table
    table_delimit = '  ' + '-' * 105 + '\n'
    tables = ''
    tables += """\n   ==> %s <==\n\n""" % ('Components')
    tables += table_delimit
    tables += """     %6s %20s %1s %-26s %3s %16s   %-s\n""" % ('', 'Method', '/', 'Basis', 'Rqd', 'Energy [Eh]', 'Variable')
    tables += table_delimit
    for job in JOBS_EXT:
        star = ''
        for mc in MODELCHEM:
            if (job['f_wfn'] == mc['f_wfn']) and (job['f_basis'] == mc['f_basis']):
                star = '*'
        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % ('', job['f_wfn'],
                  '/', job['f_basis'], star, job['f_energy'], VARH[job['f_wfn']][job['f_wfn']])
    tables += table_delimit

    tables += """\n   ==> %s <==\n\n""" % ('Stages')
    tables += table_delimit
    tables += """     %6s %20s %1s %-27s %2s %16s   %-s\n""" % ('Stage', 'Method', '/', 'Basis', 'Wt', 'Energy [Eh]', 'Scheme')
    tables += table_delimit
    for stage in GRAND_NEED:
        tables += """     %6s %20s %1s %-27s %2d %16.8f   %-s\n""" % (stage['d_stage'], stage['d_wfn'],
                  '/', stage['d_basis'], stage['d_coef'], stage['d_energy'], stage['d_scheme'].__name__)
    tables += table_delimit

    tables += """\n   ==> %s <==\n\n""" % ('CBS')
    tables += table_delimit
    tables += """     %6s %20s %1s %-27s %2s %16s   %-s\n""" % ('Stage', 'Method', '/', 'Basis', '', 'Energy [Eh]', 'Scheme')
    tables += table_delimit
    if do_scf:
        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[0]['d_stage'],
                                                                      GRAND_NEED[0]['d_wfn'], '/', GRAND_NEED[0]['d_basis'], '',
                                                                      GRAND_NEED[0]['d_energy'],
                                                                      GRAND_NEED[0]['d_scheme'].__name__)
    if do_corl:
        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[1]['d_stage'],
                                                                      GRAND_NEED[1]['d_wfn'], '/', GRAND_NEED[1]['d_basis'], '',
                                                                      GRAND_NEED[1]['d_energy'] - GRAND_NEED[2]['d_energy'],
                                                                      GRAND_NEED[1]['d_scheme'].__name__)
    if do_delta:
        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[3]['d_stage'],
                                                                      GRAND_NEED[3]['d_wfn'] + ' - ' + GRAND_NEED[4]['d_wfn'], '/', GRAND_NEED[3]['d_basis'], '',
                                                                      GRAND_NEED[3]['d_energy'] - GRAND_NEED[4]['d_energy'],
                                                                      GRAND_NEED[3]['d_scheme'].__name__)
    if do_delta2:
        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[5]['d_stage'],
                                                                      GRAND_NEED[5]['d_wfn'] + ' - ' + GRAND_NEED[6]['d_wfn'], '/', GRAND_NEED[5]['d_basis'], '',
                                                                      GRAND_NEED[5]['d_energy'] - GRAND_NEED[6]['d_energy'],
                                                                      GRAND_NEED[5]['d_scheme'].__name__)
#    if do_delta3:
#        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[6]['d_stage'], GRAND_NEED[6]['d_wfn'] + ' - ' + GRAND_NEED[7]['d_wfn'],
#                  '/', GRAND_NEED[6]['d_basis'], '', GRAND_NEED[6]['d_energy'] - GRAND_NEED[7]['d_energy'], GRAND_NEED[6]['d_scheme'].__name__)
#    if do_delta4:
#        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[8]['d_stage'], GRAND_NEED[8]['d_wfn'] + ' - ' + GRAND_NEED[9]['d_wfn'],
#                  '/', GRAND_NEED[8]['d_basis'], '', GRAND_NEED[8]['d_energy'] - GRAND_NEED[9]['d_energy'], GRAND_NEED[8]['d_scheme'].__name__)
#    if do_delta5:
#        tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % (GRAND_NEED[10]['d_stage'], GRAND_NEED[10]['d_wfn'] + ' - ' + GRAND_NEED[11]['d_wfn'],
#                  '/', GRAND_NEED[10]['d_basis'], '', GRAND_NEED[10]['d_energy'] - GRAND_NEED[11]['d_energy'], GRAND_NEED[10]['d_scheme'].__name__)
    tables += """     %6s %20s %1s %-27s %2s %16.8f   %-s\n""" % ('total', 'CBS', '', '', '', finalenergy, '')
    tables += table_delimit

    psi4.print_out(tables)

    psi4.set_variable('CBS REFERENCE ENERGY', GRAND_NEED[0]['d_energy'])
    psi4.set_variable('CBS CORRELATION ENERGY', finalenergy - GRAND_NEED[0]['d_energy'])
    psi4.set_variable('CBS TOTAL ENERGY', finalenergy)
    psi4.set_variable('CURRENT REFERENCE ENERGY', GRAND_NEED[0]['d_energy'])
    psi4.set_variable('CURRENT CORRELATION ENERGY', finalenergy - GRAND_NEED[0]['d_energy'])
    psi4.set_variable('CURRENT ENERGY', finalenergy)
    psi4.set_variable('CBS NUMBER', Njobs)

    # new skeleton wavefunction w/mol, highest-SCF basis (just to choose one), & not energy
    wfn = psi4.new_wavefunction(molecule, BSTR[-1])

    optstash.restore()

    if func == energy:
        finalquantity = finalenergy
    elif func == gradient:
        psi4.print_out('CURRENT GRADIENT')
        finalquantity = finalgradient
        wfn.set_gradient(finalquantity)
        finalquantity.print_out()
    elif func == hessian:
        psi4.print_out('CURRENT HESSIAN')
        finalquantity = finalhessian
        wfn.set_hessian(finalquantity)
        finalquantity.print_out()

    if return_wfn:
        return (finalquantity, wfn)
    else:
        return finalquantity


_lmh_labels = {1: ['HI'],
               2: ['LO', 'HI'],
               3: ['LO', 'MD', 'HI'],
               4: ['LO', 'MD', 'M2', 'HI'],
               5: ['LO', 'MD', 'M2', 'M3', 'HI']}


def expand_scheme_orders(scheme, basisname, basiszeta, wfnname, natom):
    """Check that the length of *basiszeta* array matches the implied degree of
    extrapolation in *scheme* name. Return a dictionary of same length as
    basiszeta, with *basisname* and *basiszeta* distributed therein.

    """
    if scheme == driver_util.xtpl_highest_1:
        basisname = [basisname[-1]]
        basiszeta = [basiszeta[-1]]
    Nxtpl = len(basiszeta)

    if int(scheme.__name__.split('_')[-1]) != Nxtpl:
        raise ValidationError("""Call to '%s' not valid with '%s' basis sets.""" % (scheme.__name__, len(basiszeta)))

    f_fields = ['f_wfn', 'f_basis', 'f_zeta', 'f_energy', 'f_gradient', 'f_hessian']
    NEED = {}
    for idx in range(Nxtpl):
        NEED[_lmh_labels[Nxtpl][idx]] = dict(zip(f_fields, [wfnname, basisname[idx], basiszeta[idx],
                                                            0.0,
                                                            psi4.Matrix(natom, 3),
                                                            psi4.Matrix(3 * natom, 3 * natom)]))
    return NEED


def contract_scheme_orders(needdict, datakey='f_energy'):
    """Prepared named arguments for extrapolation functions by
    extracting zetas and values (which one determined by *datakey*) out
    of *needdict* and returning a dictionary whose keys are contructed
    from _lmh_labels.

    """
    largs = {}
    largs['functionname'] = needdict['HI']['f_wfn']
    Nxtpl = len(needdict)
    zlabels = _lmh_labels[Nxtpl]  # e.g., ['LO', 'HI']

    for zeta in range(Nxtpl):
        zlab = zlabels[zeta]  # e.g., LO
        largs['z' + zlab] = needdict[zlab]['f_zeta']
        largs['value' + zlab] = needdict[zlab][datakey]

    return largs


# Quickly normalize the types for both python 2 and 3
try:
    unicode = unicode
except NameError:
    # 'unicode' is undefined, must be Python 3
    str = str
    unicode = str
    bytes = bytes
    basestring = (str, bytes)
else:
    # 'unicode' exists, must be Python 2
    str = str
    unicode = unicode
    bytes = str
    basestring = basestring

##  Aliases  ##
cbs = complete_basis_set

#################################
##  End of Complete Basis Set  ##
#################################
