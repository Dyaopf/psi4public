/*! \file
    \ingroup OPTKING
    \brief fd_geoms_freq_0(): returns geometries necessary for finite-difference
     computation of second derivatives from energies; puts undisplaced geometry last in list

  **Finite-difference formulas
  3-point - diagonal
    O(1/h^2): [f(1,0) + f(-1,0) - 2f(0,0)]/(h^2)
     [which is same as : [f(2,0) + f(-2,0) - 2f(0,0)]/(4h^2)
  3-point - off-diagonal
    O(1/h^2): new-way: [f(1,1)+f(-1,-1)+2f(0,0) -f(1,0) -f(-1,0) -f(0,1) -f(0,-1)]/(2h^2)
  
  5-point formula - diagonal
    O(1/h^4): [-f(-2,0) + 16f(-1,0) + 16f(1,0) - f(2,0) - 30f(0,0)] / (12h^2)
  5-point formula - off-diagonal
    O(1/h^4): [-1f(-1,-2) - 1f(-2,-1) + 9f(-1,-1) - 1f(+1,-1)
      - 1f(-1,1) + 9f(+1,+1) - 1f(+2,+1) - 1f(1,2)
      + 1f(-2,0) - 7f(-1,0)  - 7f(+1,0) + 1f(+2,0)
      + 1f(0,-2) - 7f(0,-1)  - 7f(0,+1) + 1f(0,+2) + 12f(0,0)]/(12h^2)
*/

#include <sstream>
#include <vector>
#include <libmints/mints.h>
#include <psi4-dec.h>

namespace psi { namespace findif {

template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}

void displace_cart(boost::shared_ptr<Matrix> geom, const CdSalcList & salclist,
  int salc_i, int disp_factor, double disp_size);
void displace_cart(boost::shared_ptr<Matrix> geom, const CdSalcList & salclist,
  int salc_i, int salc_j, int disp_factor_i, int disp_factor_j, double disp_size);

std::vector< boost::shared_ptr<Matrix> > fd_geoms_freq_0(Options &options) {

  fprintf(outfile,"\tUsing finite-differences of energies to determine vibrational frequencies");
  fprintf(outfile,"\t and normal modes.\n  ** Results are valid only at stationary points! **\n");

  int pts = options.get_int("POINTS");
  fprintf(outfile,"\tGenerating geometries for use with %d-point formula.\n", pts);
  if (pts != 3 && pts != 5)
    throw PsiException("FINDIF: Invalid number of points!",__FILE__,__LINE__);

  double disp_size = options.get_double("DISP_SIZE");
  fprintf(outfile,"\tDisplacement size will be %6.2e.\n", disp_size);

  const boost::shared_ptr<Molecule> mol = psi::Process::environment.molecule();

  // Get SALCS from libmints: all modes with rotations and translations projected out
  boost::shared_ptr<MatrixFactory> fact;
  CdSalcList salc_list(mol, fact);

  int Natom = mol->natom();
  fprintf(outfile,"\tNumber of atoms is %d.\n", Natom);

  int Nirrep = salc_list.nirrep();
  fprintf(outfile,"\tNumber of irreps is %d.\n", Nirrep);

  int Nsalc_all = salc_list.ncd();
  fprintf(outfile,"\tTotal Number of SALCS is %d.\n", Nsalc_all);
  //for (int i=0; i<Nsalc_all; ++i)
  //  salc_list[i].print();

  // build vectors that list indices of salcs for each irrep
  std::vector< std::vector<int> > salcs_pi; // salcs per irrep
  for (int h=0; h<Nirrep; ++h)
    salcs_pi.push_back( std::vector<int>() );
  for (int i=0; i<Nsalc_all; ++i)
    salcs_pi[salc_list[i].irrep()].push_back(i);

  fprintf(outfile,"\tIndex of salcs per irrep:\n");
  for (int h=0; h<Nirrep; ++h) {
    fprintf(outfile, "\t %d : ", h+1);
    for (int i=0; i<salcs_pi[h].size(); ++i)
      fprintf(outfile," %d ", salcs_pi[h][i]);
    fprintf(outfile, "\n");
  }

  // From now on in code, salcs_pi establishes the canonical order of SALCs and displacements
  fprintf(outfile,"\tNumber of SALC's per irrep:\n");
  for (int h=0; h<Nirrep; ++h)
    fprintf(outfile,"\t\t Irrep %d: %d\n", h+1, (int) salcs_pi[h].size());

  // Determine number of displacements
  std::vector<int> Ndisp_pi (Nirrep);

  // diagonal displacements for symmetric coordinates
  if (pts == 3)
    Ndisp_pi[0] = 2 * salcs_pi[0].size();
  else if (pts == 5)
    Ndisp_pi[0] = 4 * salcs_pi[0].size();

  // diagonal displacements for asymmetric coordinates
  for (int h=1; h<Nirrep; ++h) {
    if (pts == 3)
      Ndisp_pi[h] = salcs_pi[h].size();
    else if (pts == 5)
      Ndisp_pi[h] = 2* salcs_pi[h].size();
  }

  // off-diagonal displacements
  for (int h=0; h<Nirrep; ++h) {
    if (pts == 3)
      Ndisp_pi[h] += 2 * salcs_pi[h].size() * (salcs_pi[h].size() - 1) / 2;
    else if (pts == 5)
      Ndisp_pi[h] += 8 * salcs_pi[h].size() * (salcs_pi[h].size() - 1) / 2;
  }

  int Ndisp_all = 0;
  for (int h=0; h<Nirrep; ++h)
    Ndisp_all += Ndisp_pi[h];

  fprintf(outfile,"\tTotal number of displacements is %d.\n", Ndisp_all);
  fprintf(outfile,"\tNumber of displacements per irrep:\n");
  for (int h=0; h<Nirrep; ++h)
    fprintf(outfile,"\t\t Irrep %d: %d\n", h+1, Ndisp_pi[h]);

  // Get reference geometry
  Matrix ref_geom_temp = mol->geometry();
  SharedMatrix ref_geom(ref_geom_temp.clone());
  ref_geom->set_name("Reference geometry");

  // to be returned and converted into "matrix_vector" list in python
  std::vector< boost::shared_ptr<Matrix> > disp_geoms;

  for (int h=0; h<Nirrep; ++h) { // loop over irreps

    for (int i=0; i<salcs_pi[h].size(); ++i) { // loop over salcs of this irrep
      int salc_i = salcs_pi[h][i];   // index in cdsalc of this salc

      if (h == 0) { // symmetric diagonal displacements (first in list)
        if (pts == 3) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, -1, disp_size);
          disp_geoms.push_back(geom1);

          boost::shared_ptr<Matrix> geom2(ref_geom->clone());
          displace_cart(geom2, salc_list, salc_i, +1, disp_size);
          disp_geoms.push_back(geom2);
        }
        else if (pts == 5) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, -2, disp_size);
          disp_geoms.push_back(geom1);

          boost::shared_ptr<Matrix> geom2(ref_geom->clone());
          displace_cart(geom2, salc_list, salc_i, -1, disp_size);
          disp_geoms.push_back(geom2);

          boost::shared_ptr<Matrix> geom3(ref_geom->clone());
          displace_cart(geom3, salc_list, salc_i, +1, disp_size);
          disp_geoms.push_back(geom3);

          boost::shared_ptr<Matrix> geom4(ref_geom->clone());
          displace_cart(geom4, salc_list, salc_i, +2, disp_size);
          disp_geoms.push_back(geom4);
        }
      }
      else { // h != 0; assymmetric diagonal displacements
        if (pts == 3) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, -1, disp_size);
          disp_geoms.push_back(geom1);
        }
        else if (pts == 5) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, -2, disp_size);
          disp_geoms.push_back(geom1);

          boost::shared_ptr<Matrix> geom2(ref_geom->clone());
          displace_cart(geom2, salc_list, salc_i, -1, disp_size);
          disp_geoms.push_back(geom2);
        }
      }
    } // i, salcs of this irrep

    // off-diagonal displacements
    for (int i=0; i<salcs_pi[h].size(); ++i) { // loop over salcs of this irrep
      int salc_i = salcs_pi[h][i];     // index in cdsalc of this salc

      for (int j=0; j<i; ++j) {        // loop over salcs of this irrep
        int salc_j = salcs_pi[h][j];   // index in cdsalc of this salc

        if (pts == 3) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, salc_j, +1, +1, disp_size);
          disp_geoms.push_back(geom1);

          boost::shared_ptr<Matrix> geom2(ref_geom->clone());
          displace_cart(geom2, salc_list, salc_i, salc_j, -1, -1, disp_size);
          disp_geoms.push_back(geom2);
        }
        else if (pts == 5) {
          boost::shared_ptr<Matrix> geom1(ref_geom->clone());
          displace_cart(geom1, salc_list, salc_i, salc_j, -1, -2, disp_size);
          disp_geoms.push_back(geom1);

          boost::shared_ptr<Matrix> geom2(ref_geom->clone());
          displace_cart(geom2, salc_list, salc_i, salc_j, -2, -1, disp_size);
          disp_geoms.push_back(geom2);

          boost::shared_ptr<Matrix> geom3(ref_geom->clone());
          displace_cart(geom3, salc_list, salc_i, salc_j, -1, -1, disp_size);
          disp_geoms.push_back(geom3);

          boost::shared_ptr<Matrix> geom4(ref_geom->clone());
          displace_cart(geom4, salc_list, salc_i, salc_j, +1, -1, disp_size);
          disp_geoms.push_back(geom4);

          boost::shared_ptr<Matrix> geom5(ref_geom->clone());
          displace_cart(geom5, salc_list, salc_i, salc_j, -1, +1, disp_size);
          disp_geoms.push_back(geom5);

          boost::shared_ptr<Matrix> geom6(ref_geom->clone());
          displace_cart(geom6, salc_list, salc_i, salc_j, +1, +1, disp_size);
          disp_geoms.push_back(geom6);

          boost::shared_ptr<Matrix> geom7(ref_geom->clone());
          displace_cart(geom7, salc_list, salc_i, salc_j, +2, +1, disp_size);
          disp_geoms.push_back(geom7);

          boost::shared_ptr<Matrix> geom8(ref_geom->clone());
          displace_cart(geom8, salc_list, salc_i, salc_j, +1, +2, disp_size);
          disp_geoms.push_back(geom8);
        } // pts == 5
      } // m, salc_j
    } // i, salc_i 
  } // h, irreps

  // put reference geometry list in list
  disp_geoms.push_back(ref_geom);

  return disp_geoms;
}

// displaces from a reference geometry: geom += salclist[salc_i] * disp_i * disp_size
void displace_cart(boost::shared_ptr<Matrix> geom, const CdSalcList & salclist,
  int salc_i, int disp_factor, double disp_size) {

  geom->set_name("Coord: " + to_string(salc_i) + ", Disp: " + to_string(disp_factor));

  const boost::shared_ptr<Molecule> mol = psi::Process::environment.molecule();

  int nc = salclist[salc_i].ncomponent();

  for (int c=0; c<nc; ++c) {
    int a          = salclist[salc_i].component(c).atom;
    int xyz        = salclist[salc_i].component(c).xyz;
    double coef    = salclist[salc_i].component(c).coef;

    geom->add(0, a, xyz, disp_factor * disp_size * coef / sqrt(mol->mass(a)));
  }

  return;
}

// displaces from a reference geometry.
// geom += salclist[salc_i] * disp_i * disp_size + salclist[salc_j] * disp_j * disp_size
void displace_cart(boost::shared_ptr<Matrix> geom, const CdSalcList & salclist,
  int salc_i, int salc_j, int disp_factor_i, int disp_factor_j, double disp_size) {

  geom->set_name("Coord: " + to_string(salc_i) + ", Disp: " + to_string(disp_factor_i)
    + "Coord: " + to_string(salc_j) + ", Disp: " + to_string(disp_factor_j));

  const boost::shared_ptr<Molecule> mol = psi::Process::environment.molecule();

  int a, xyz;
  double coef;

  for (int c=0; c<salclist[salc_i].ncomponent(); ++c) {
    a    = salclist[salc_i].component(c).atom;
    xyz  = salclist[salc_i].component(c).xyz;
    coef = salclist[salc_i].component(c).coef;

    geom->add(0, a, xyz, disp_factor_i * disp_size * coef / sqrt(mol->mass(a)));
  }

  for (int c=0; c<salclist[salc_j].ncomponent(); ++c) {
    a    = salclist[salc_j].component(c).atom;
    xyz  = salclist[salc_j].component(c).xyz;
    coef = salclist[salc_j].component(c).coef;

    geom->add(0, a, xyz, disp_factor_j * disp_size * coef / sqrt(mol->mass(a)));
  }

  return;
}

}}

