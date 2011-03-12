#ifndef _psi_src_lib_libmints_angularmomentum_h_
#define _psi_src_lib_libmints_angularmomentum_h_

#include <boost/shared_ptr.hpp>

namespace psi {

class BasisSet;
class GaussianShell;
class ObaraSaikaTwoCenterRecursion;
class OneBodyAOInt;
class IntegralFactory;
class SphericalTransform;
class SimpleMatrix;

/*! \ingroup MINTS
 *  \class DipoleInt
 *  \brief Computes dipole integrals.
 *
 * Use an IntegralFactory to create this object. */
class AngularMomentumInt : public OneBodyAOInt
{
    //! Obara and Saika recursion object to be used.
    ObaraSaikaTwoCenterRecursion overlap_recur_;

    //! Computes the dipole between two gaussian shells.
    void compute_pair(const boost::shared_ptr<GaussianShell>&, const boost::shared_ptr<GaussianShell>&);
    //! Computes the dipole derivative between two gaussian shells.
//    void compute_pair_deriv1(const boost::shared_ptr<GaussianShell>&, const boost::shared_ptr<GaussianShell>&);

public:
    //! Constructor. Do not call directly use an IntegralFactory.
    AngularMomentumInt(std::vector<SphericalTransform>&, boost::shared_ptr<BasisSet>, boost::shared_ptr<BasisSet>, int deriv=0);
    //! Virtual destructor
    virtual ~AngularMomentumInt();

    //! Does the method provide first derivatives?
    bool has_deriv1() { return true; }
};

}

#endif
