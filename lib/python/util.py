import PsiMod
import sys
from psiexceptions import *

def set_memory(bytes):
    PsiMod.set_memory(bytes)

def get_memory():
    return PsiMod.get_memory()

def set_num_threads(nthread):
    PsiMod.set_n_threads(nthread)

def get_num_threads():
    return PsiMol.get_n_threads()

# Test functions
def compare_values(expected, computed, digits, label):
    if (abs(expected-computed) > 10**(-digits)):
        print "\t%s: computed value (%f) does not match (%f) to %d digits." % (label, expected, computed, digits)
        sys.exit(1)

    print "\t%s: matched." % (label)

def compare_strings(expected, computed, label):
    if(expected != computed):
        print "\t%s: computed value (%s) does not match (%s)." % (label, expected, computed)
        sys.exit(1)

    print "\t%s: matched." % (label)

def compare_matrices(expected, computed, digits, label):
    if (expected.nirrep() != computed.nirrep()):
        print "\t%s has %d irreps, but %s has %d\n." % (expected.name(), expected.nirrep(), computed.name(), computed.nirrep())
        sys.exit(1)
    nirreps = expected.nirrep()
    for irrep in range(nirreps):
        if(expected.rows(irrep) != computed.rows(irrep)):
            print "\t%s has %d rows in irrep %d, but %s has %d\n." % (expected.name(), expected.rows(irrep), irrep, computed.name(), computed.rows(irrep))
            sys.exit(1)
        if(expected.cols(irrep) != computed.cols(irrep)):
            print "\t%s has %d columns in irrep, but %s has %d\n." % (expected.name(), expected.cols(irrep), irrep, computed.name(), computed.cols(irrep))
            sys.exit(1)
        rows = expected.rows(irrep)
        cols = expected.cols(irrep)
        failed = 0;
        for row in range(rows):
            for col in range(cols):
                if(abs(expected.get(irrep, row, col) - computed.get(irrep, row, col)) > 10**(-digits)):
                     print "\t%s: computed value (%s) does not match (%s)." % (label, expected.get(irrep, row, col) , computed.get(irrep, row, col))
                     failed = 1
                     break

        if(failed):
            PsiMod.print_out("The Failed Test Matrices\n")
            computed.print_out()
            expected.print_out()
            sys.exit(1)

    print "\t%s: matched." % (label)

def compare_vectors(expected, computed, digits, label):
    if (expected.nirrep() != computed.nirrep()):
        print "\t%s has %d irreps, but %s has %d\n." % (expected.name(), expected.nirrep(), computed.name(), computed.nirrep())
        sys.exit(1)
    nirreps = expected.nirrep()
    for irrep in range(nirreps):
        if(expected.dim(irrep) != computed.dim(irrep)):
            print "\tThe reference has %d entries in irrep %d, but the computed vector has %d\n." % (expected.dim(irrep), irrep, computed.dim(irrep))
            sys.exit(1)
        dim = expected.dim(irrep)
        failed = 0;
        for entry in range(dim):
            if(abs(expected.get(irrep, entry) - computed.get(irrep, entry)) > 10**(-digits)):
                 print "\t%s: computed value (%s) does not match (%s)." % (label, expected.get(irrep, entry) , computed.get(irrep, entry))
                 failed = 1
                 break

        if(failed):
            PsiMod.print_out("The computed vector\n")
            computed.print_out()
            PsiMod.print_out("The reference vector\n")
            expected.print_out()
            sys.exit(1)

    print "\t%s: matched." % (label)
