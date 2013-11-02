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

#ifndef SMOOTHINGJUMP_H
#define SMOOTHINGJUMP_H

#include "InternalSideUserObject.h"

class SmoothingFunction;

template<>
InputParameters validParams<SmoothingFunction>();

/**
 *
 */
class SmoothingFunction : public InternalSideUserObject
{
public:
  SmoothingFunction(const std::string & name, InputParameters parameters);
  virtual ~SmoothingFunction();

  virtual void initialize();
  virtual void execute();
  virtual void destroy();
  virtual void finalize();
  virtual void threadJoin(const UserObject & uo);

  Real getValue() const { return _value; }

protected:
    // Auxiliary system variable:
    AuxiliarySystem & _aux;
    
    // Gradient value:
    VariableValue & _variable;
    VariableValue & _variable_neighbor;
    
    // Name of the variable:
    std::string _name;
    
    // Value
    Real _value;
};

#endif /* SMOOTHINGJUMP_H */
