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

#ifndef BADGERFLUX_H
#define BADGERFLUX_H

#include "Kernel.h"

class BadgerFlux;

template<>
InputParameters validParams<BadgerFlux>();
class BadgerFlux : public Kernel
{
public:

  BadgerFlux(const std::string & name,
             InputParameters parameters);

protected:
 
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian( unsigned int jvar );

private:
    // Boolean
    const bool & _isImplicit;
    
    // Variable
    VariableValue & _u_old;
};

#endif // BadgerFlux_H
