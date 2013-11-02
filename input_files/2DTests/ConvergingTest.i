#
#####################################################
# Define some global parameters used in the blocks. #
#####################################################
#

[GlobalParams]
###### Other parameters #######
order = FIRST
viscosity_name = ENTROPY
isJumpOn = false
Ce = 1.

[]

#############################################################################
#                          USER OBJECTS                                     #
#############################################################################
# Define the user object class that store the EOS parameters.               #
#############################################################################

[UserObjects]

  [./JumpGradUS]
    type = JumpGradientInterface
    variable = u
    jump_name = jump_grad_us_aux
    execute_on = timestep_begin
  [../]

[]

###### Mesh #######
[Mesh]
  type = GeneratedMesh
  uniform_refine = 0
  dim = 2
  nx = 320
  ny = 320
  xmin = 0
  xmax = 3.14159
  ymin = 0
  ymax = 3.14159
  block_id = '0'
#elem_type = QUAD9
[]

#############################################################################
#                             VARIABLES                                     #
#############################################################################
# Define the variables we want to solve for: l=liquid phase and g=gas phase.#
#############################################################################

[Variables]
  [./u]
    family = LAGRANGE
    scaling = 1e+1
	[./InitialCondition]
        type = FunctionIC
        function = exact_fn
	[../]
  [../]
[]

############################################################################################################
#                                            KERNELS                                                       #
############################################################################################################
# Define the kernels for time dependent, convection and viscosity terms. Same index as for variable block. #
############################################################################################################

[Kernels]

  [./Time]
    type = BadgerTime
    variable = u
  [../]

  [./Flux]
    type = BadgerFlux
    variable = u
  [../]

  [./ViscFlux]
    type = BadgerViscFlux
    variable = u
  [../]

  [./ForcingTerm]
    type = BadgerForcingTerm
    variable = u
  [../]
[]

##############################################################################################
#                                       AUXILARY VARIABLES                                   #
##############################################################################################
# Define the auxilary variables                                                              #
##############################################################################################

[AuxVariables]
   [./s_aux]
      family = LAGRANGE
   [../]

   [./us_aux]
      family = LAGRANGE
   [../]

   [./mu_max_aux]
    family = MONOMIAL
    order = CONSTANT
   [../]

   [./mu_aux]
    family = MONOMIAL
    order = CONSTANT
   [../]

  [./exact_sol_aux]
    family = LAGRANGE
    order = FIRST
  [../]

  [./jump_grad_us_aux]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

##############################################################################################
#                                       AUXILARY KERNELS                                     #
##############################################################################################
# Define the auxilary kernels for liquid and gas phases. Same index as for variable block.   #
##############################################################################################
[AuxKernels]

  [./EntropyAK]
    type = EntropyAux
    variable = s_aux
    u = u
  [../]

  [./UtimesEntropyAK]
    type = UtimesEntropyAux
    variable = us_aux
    u = u
    s = s_aux
  [../]

  [./MuMaxAK]
    type = MaterialRealAux
    variable = mu_max_aux
    property = mu_max
  [../]

   [./MuAK]
    type = MaterialRealAux
    variable = mu_aux
    property = mu
   [../]

   [./ExactSolution]
    type = FunctionAux
    variable = exact_sol_aux
    function = exact_fn
   [../]

[]

##############################################################################################
#                                       MATERIALS                                            #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################

[Materials]
#active = ''
  [./EntViscMat]
    type = BadgerComputeViscCoeff
    block = '0'
    u = u
    s = s_aux
    us = us_aux
    jump_grad_us = jump_grad_us_aux
    PPS_name = AverageEntropy
  [../]

[]

##############################################################################################
#                                     PPS                                                    #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################
[Postprocessors]

  [./AverageEntropy]
    type = ElementAverageValue
    variable = s_aux
    execute_on = timestep_begin
  [../]

  [./l2_err]
    type = ElementL2Error
    variable = u
    function = exact_fn
  [../]
[]

##############################################################################################
#                                   Functions                                                #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################
[Functions]
  [./exact_fn]
    type = ParsedFunction
    value=sin(x+y-t)+2.
  [../]
[]

##############################################################################################
#                               BOUNDARY CONDITIONS                                          #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################
[BCs]
  [./RightDBC]
    type = FunctionDirichletBC
    variable = u
    function = exact_fn
    boundary = '0 1 2 3'
  [../]
[]

##############################################################################################
#                                  PRECONDITIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Preconditioning]
#  active = 'FDP_Newton'
  active = 'SMP_Newton'

  [./FDP_Newton]
    type = FDP
    full = true
    solve_type = 'PJFNK'
#petsc_options = '-snes_mf_operator -snes_ksp_ew'
#petsc_options_iname = '-mat_fd_coloring_err  -mat_fd_type  -mat_mffd_type'
#petsc_options_value = '1.e-12       ds             ds'
  [../]

  [./SMP_Newton]
    type = SMP
    full = true
    solve_type = 'PJFNK'
  [../]
[]

##############################################################################################
#                                     EXECUTIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Executioner]
  type = Transient
  scheme = 'bdf2' # 'implicit-rk'
  end_time = 2.
  #num_steps = 100
  dt = 0.00625
  dtmin = 1e-9
  l_tol = 1e-8
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-5
  l_max_its = 54
  nl_max_its = 10
  [./Quadrature]
    type = TRAP
  [../]
[]
##############################################################################################
#                                        OUTPUT                                              #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Output]
  output_initial = true
  output_displaced = false
  postprocessor_screen = true
  interval = 1
  exodus = true
  perf_log = true
[]
