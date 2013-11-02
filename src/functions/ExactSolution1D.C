/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ExactSolution1D.h"

template<>
InputParameters validParams<ExactSolution1D>()
{
  InputParameters params = validParams<Function>();
  return params;
}

ExactSolution1D::ExactSolution1D(const std::string & name, InputParameters parameters) :
  Function(name, parameters)
{}

ExactSolution1D::~ExactSolution1D()
{}

Real
ExactSolution1D::value(const Real time, const Point & pt)
{
    // Initialize value:
    Real _u = std::sin(2*libMesh::pi*pt(0));
    
    // Compute the intial residual and its derivative:
    Real _res = _u - std::sin(2*libMesh::pi*(pt(0)-_u*time));
    Real _res_prime = 1 + 2*libMesh::pi*time*std::cos(2*libMesh::pi*(pt(0)-_u*time));
    
    // Loop until residual is converged:
    while (_res > 1.e-3) {
        //_u = std::sin(2*libMesh::pi*(pt(0)-_u*time));
        _u -= _res/_res_prime;
        _res = _u - std::sin(2*libMesh::pi*(pt(0)-_u*time));
        _res_prime = 1 + 2*libMesh::pi*time*std::cos(2*libMesh::pi*(pt(0)-_u*time));
    }
    
    // Return the value:
    //std::cout<<"u="<<_u<<std::endl;
    return _u;
}

