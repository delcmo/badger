#ifndef BADGERSBCS_H
#define BADGERSBCS_H

#include "IntegratedBC.h"

// Forward Declarations
class BadgersBCs;

template<>
InputParameters validParams<BadgersBCs>();

class BadgersBCs : public IntegratedBC
{

public:
  BadgersBCs(const std::string & name, InputParameters parameters);

  virtual ~BadgersBCs(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
    
private:
    // Implicit:
    const bool & _isImplicit;
    
    // Old value:
    VariableValue & _u_old;

};

#endif // BADGERSBCS_H

