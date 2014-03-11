#ifndef BADGERCOMPUTEVISCCOEFF_H
#define BADGERCOMPUTEVISCCOEFF_H

#include "Material.h"
#include "MaterialProperty.h"

//Forward Declarations
class BadgerComputeViscCoeff;

template<>
InputParameters validParams<BadgerComputeViscCoeff>();

class BadgerComputeViscCoeff : public Material
{
public:
  BadgerComputeViscCoeff(const std::string & name, InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();  
  virtual void computeQpProperties();

private:
    // implicit or explicit:
    const bool & _isImplicit;
    
    // Viscosity type:
    const std::string & _visc_name;
    
    // Boolean for jump
    const bool & _isJumpOn;
    
    // Coupled variables:
    VariableValue & _u;
    VariableValue & _s;
    VariableValue & _s_old;
    VariableValue & _s_older;
    VariableGradient & _grad_us;
    VariableGradient & _grad_us_old;
    
    // Variable: jump
    VariableValue & _jump_grad_u;
    VariableValue & _jump_grad_u_old;
    
    // Material properties
    MaterialProperty<Real> & _mu;
    //MaterialProperty<Real> & _mu_old;
    MaterialProperty<Real> & _mu_max;
    
    // Multiplicative coefficient for viscosity:
    const double & _Ce;
    
    // Name of the posprocessors for pressure and velocity:
    std::string _pps_name;
};

#endif //BADGERCOMPUTEVISCCOEFF_H
