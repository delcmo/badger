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

#ifndef BADGERFLUXNOTINTEGRATED_H
#define BADGERFLUXNOTINTEGRATED_H

#include "Kernel.h"

class BadgerFluxNotIntegrated;

template<>
InputParameters validParams<BadgerFluxNotIntegrated>();
class BadgerFluxNotIntegrated : public Kernel
{
public:

  BadgerFluxNotIntegrated(const std::string & name,
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
    VariableGradient & _grad_u_old;
};

#endif // BadgerFluxNotIntegrated_H
