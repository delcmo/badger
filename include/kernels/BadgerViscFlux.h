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

#ifndef BADGERVISCFLUX_H
#define BADGERVISCFLUX_H

#include "Kernel.h"

class BadgerViscFlux;

template<>
InputParameters validParams<BadgerViscFlux>();
class BadgerViscFlux : public Kernel
{
public:

  BadgerViscFlux(const std::string & name,
             InputParameters parameters);

protected:
 
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian( unsigned int jvar );

private:
    // Implicit scheme:
    const bool & _isImplicit;
    
    // Old value:
    VariableGradient & _grad_u_old;
    
    // Material property:
    MaterialProperty<Real> & _mu;
    
};

#endif // BadgerViscFlux_H
