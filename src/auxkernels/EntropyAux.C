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
/**
This function computes the entropy used in the definition of the entropy viscosity coefficient.
**/
#include "EntropyAux.h"

template<>
InputParameters validParams<EntropyAux>()
{
  InputParameters params = validParams<AuxKernel>();
    params.addParam<bool>("isImplicit", true, "implicit or explicit schemes.");
    params.addRequiredCoupledVar("u", "variable it is solved for.");
    params.addParam<int>("type_ent", 1, "type of entropy function to use.");
  return params;
}

EntropyAux::EntropyAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    // Implicit
    _isImplicit(getParam<bool>("isImplicit")),
    // Coupled variable:
    _u(_isImplicit ? coupledValue("u") : coupledValueOld("u")),
    // Entropy type:
    _type_ent(getParam<int>("type_ent"))
{}

Real
EntropyAux::computeValue()
{
    switch (_type_ent) {
        case 1:
            return 0.5*_u[_qp]*_u[_qp];
            break;
            
        default:
            return 0.;
            break;
    }
}
