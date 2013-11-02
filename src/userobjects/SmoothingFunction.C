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

#include "SmoothingFunction.h"

/* This function is called to compute the jump of the gradient of a given quantity when using CONTINUOUS finite element. This function acts on the sides of the cell.*/
template<>
InputParameters validParams<SmoothingFunction>()
{
  InputParameters params = validParams<InternalSideUserObject>();
    params.addRequiredCoupledVar("variable", "the variable name this userobject is acting on.");
    params.addRequiredParam<std::string>("name_var", "name of the variable.");
  return params;
}

SmoothingFunction::SmoothingFunction(const std::string & name, InputParameters parameters) :
    InternalSideUserObject(name, parameters),
    _aux(_fe_problem.getAuxiliarySystem()),
    _variable(coupledValue("variable")),
    _variable_neighbor(coupledNeighborValue("variable")),
    _name(getParam<std::string>("name_var")),
    _value(0.)
{
}

SmoothingFunction::~SmoothingFunction()
{
}

void
SmoothingFunction::initialize()
{
    // Set _aux:
    NumericVector<Number> & sln = _aux.solution();
    _aux.system().zero_variable(sln, _aux.getVariable(_tid, _name).number());
    
}

void
SmoothingFunction::execute()
{
    // Smooth the variable:
    Real _value = 0.;
    NumericVector<Number> & sln = _aux.solution();
    for (unsigned int qp = 0; qp < _q_point.size(); ++qp)
        _value = 0.5 * ( _variable[qp] + _variable_neighbor[qp] );
    
    // Get the degree of freedom for each cell:
    dof_id_type _dof_nb = _current_elem->dof_number(_aux.number(), _fe_problem.getVariable(_tid, _name).number(), 0);
    dof_id_type _dof_nb_neighbor = _neighbor_elem->dof_number(_aux.number(), _fe_problem.getVariable(_tid, _name).number(), 0);
    
    // Set the variable with its smoother value:
    sln.add(_dof_nb, _value);
    sln.add(_dof_nb_neighbor, _value);
}

void
SmoothingFunction::destroy()
{
}

void
SmoothingFunction::finalize()
{
    _aux.solution().close();
    
}

void
SmoothingFunction::threadJoin(const UserObject & uo)
{
}
