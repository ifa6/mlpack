/*
 * =====================================================================================
 * 
 *       Filename:  gop_nmf.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  07/19/2008 12:05:37 AM EDT
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Nikolaos Vasiloglou (NV), nvasil@ieee.org
 *        Company:  Georgia Tech Fastlab-ESP Lab
 * 
 * =====================================================================================
 */
#ifndef GOP_NMF_ENGINE_H_
#define GOP_NMF_ENGINE_H_
#include <map>
#include "fastlib/fastlib.h"
#include "../l_bfgs/l_bfgs.h"
#include "geometric_nmf.h"


class RelaxedNmf {
 public:
  void Init(ArrayList<index_t> &rows,
            ArrayList<index_t> &columns,
            ArrayList<double> &values,
            index_t new_dim, // new dimension of the factorization
            double grad_tolerance, // if the norm gradient is less than the tolerance
                                   // then it terminates
            Matrix &x_lower_bound, // the initial lower bound for x (optimization variable)
            Matrix &x_upper_bound  // the initial upper bound for x (optimization variable)
           );
  void Init(fx_module *module,
            ArrayList<index_t> &rows,
            ArrayList<index_t> &columns,
            ArrayList<double> &values,
            Matrix &x_lower_bound, // the initial lower bound for x (optimization variable)
            Matrix &x_upper_bound  // the initial upper bound for x (optimization variable)
           );
  void Destruct();
  // The following are required by LBFGS
  void ComputeGradient(Matrix &coordinates, Matrix *gradient);
  void ComputeObjective(Matrix &coordinates, double *objective);
  // This class implements a convex relaxation of the nmf objective
  // At some point we need to compute the original objective the non relaxed
  void ComputeNonRelaxedObjective(Matrix &coordinates, double *objective);
  void ComputeFeasibilityError(Matrix &coordinates, double *error);
  double ComputeLagrangian(Matrix &coordinates);
  void UpdateLagrangeMult(Matrix &coordinates);
  void Project(Matrix *coordinates);
  void set_sigma(double sigma); 
  void GiveInitMatrix(Matrix *init_data);
	bool IsDiverging(double objective); 
  bool IsOptimizationOver(Matrix &coordinates, Matrix &gradient, double step);
  bool IsIntermediateStepOver(Matrix &coordinates, Matrix &gradient, double step);
  
  // The following are required by the branch and bound
  double GetSoftLowerBound();
  bool IsInfeasible();
    
 private:
  // number of rows of the original matrix
  index_t num_of_rows_;
  // number of columns of the original matrix
  index_t num_of_columns_;
  // offset of the H matrix on the coordinate variable
  index_t h_offset_;
  index_t w_offset_;
  double values_sq_norm_;
  index_t new_dimension_;
  // constant term for the LP relaxation part
  Vector a_linear_term_;
  // linear term for the LP relaxation part
  Vector b_linear_term_;
  ArrayList<index_t> rows_;
  ArrayList<index_t> columns_;
  ArrayList<double> values_;
  // lower bound for the optimization variable
  Matrix x_lower_bound_;
  // upper bound for the optimization variable
  Matrix x_upper_bound_;
  // soft lower bound of the relaxation
  double soft_lower_bound_;
  // tolerance for the gradient norm
  double grad_tolerance_;
  double previous_objective_;

  inline double ComputeExpTaylorApproximation(double x, index_t order);
  index_t ComputeExpTaylorOrder(double error);
};

/*
 * The diffetence from the previous one is the use of log barriers for the bounds
 * but it is too slow
 */
class RelaxedNmf1 {
 public:
  void Init(ArrayList<index_t> &rows,
            ArrayList<index_t> &columns,
            ArrayList<double> &values,
            index_t new_dim, // new dimension of the factorization
            double grad_tolerance, // if the norm gradient is less than the tolerance
                                   // then it terminates
            Matrix &x_lower_bound, // the initial lower bound for x (optimization variable)
            Matrix &x_upper_bound  // the initial upper bound for x (optimization variable)
           );
  void Destruct();
  // The following are required by LBFGS
  void ComputeGradient(Matrix &coordinates, Matrix *gradient);
  void ComputeObjective(Matrix &coordinates, double *objective);
  // This class implements a convex relaxation of the nmf objective
  // At some point we need to compute the original objective the non relaxed
  void ComputeNonRelaxedObjective(Matrix &coordinates, double *objective);
  void ComputeFeasibilityError(Matrix &coordinates, double *error);
  double ComputeLagrangian(Matrix &coordinates);
  void UpdateLagrangeMult(Matrix &coordinates);
  void Project(Matrix *coordinates);
  void set_sigma(double sigma); 
  void GiveInitMatrix(Matrix *init_data);
	bool IsDiverging(double objective); 
  bool IsOptimizationOver(Matrix &coordinates, Matrix &gradient, double step);
  bool IsIntermediateStepOver(Matrix &coordinates, Matrix &gradient, double step);
  
  // The following are required by the branch and bound
  double GetSoftLowerBound();
    
 private:
  // number of rows of the original matrix
  index_t num_of_rows_;
  // number of columns of the original matrix
  index_t num_of_columns_;
  // offset of the H matrix on the coordinate variable
  index_t h_offset_;
  index_t w_offset_;
  double values_sq_norm_;
  index_t new_dimension_;
  // constant term for the LP relaxation part
  Vector a_linear_term_;
  // linear term for the LP relaxation part
  Vector b_linear_term_;
  ArrayList<index_t> rows_;
  ArrayList<index_t> columns_;
  ArrayList<double> values_;
  // lower bound for the optimization variable
  Matrix x_lower_bound_;
  // upper bound for the optimization variable
  Matrix x_upper_bound_;
  // soft lower bound of the relaxation
  double soft_lower_bound_;
  // tolerance for the gradient norm
  double grad_tolerance_;
  //  the penalty barrier
  double sigma_;
};

class RelaxedNmfIsometric {
 public:
  void Init(fx_module *module,
                      ArrayList<index_t> &rows,
                      ArrayList<index_t> &columns,
                      ArrayList<double> &values,
                      Matrix &x_lower_bound, 
                      Matrix &x_upper_bound);
  void Destruct();
  void SetOptVarRowColumn(index_t row, index_t column);
  void SetOptVarSign(double sign);
  // The following are required by LBFGS
  void ComputeGradient(Matrix &coordinates, Matrix *gradient);
  void ComputeObjective(Matrix &coordinates, double *objective);
  // This class implements a convex relaxation of the nmf objective
  // At some point we need to compute the original objective the non relaxed
  void ComputeNonRelaxedObjective(Matrix &coordinates, double *objective);
  void ComputeFeasibilityError(Matrix &coordinates, double *error);
  double ComputeLagrangian(Matrix &coordinates);
  void UpdateLagrangeMult(Matrix &coordinates);
  void Project(Matrix *coordinates);
  void set_sigma(double sigma); 
  void GiveInitMatrix(Matrix *init_data);
	bool IsDiverging(double objective); 
  bool IsOptimizationOver(Matrix &coordinates, Matrix &gradient, double step);
  bool IsIntermediateStepOver(Matrix &coordinates, Matrix &gradient, double step);
   
  // The following are required by the branch and bound
  double GetSoftLowerBound();
  bool IsInfeasible();
    
 private:
  // holds all the info
  fx_module *module_;
  // number of rows of the original matrix
  index_t num_of_rows_;
  // number of columns of the original matrix
  index_t num_of_columns_;
  // offset of the H matrix on the coordinate variable
  index_t h_offset_;
  index_t w_offset_;
  double values_sq_norm_;
  index_t new_dimension_;
  double desired_duality_gap_;
  ArrayList<std::pair<index_t, index_t> > nearest_neighbor_pairs_;
  ArrayList<double> nearest_distances_;
  index_t num_of_nearest_pairs_;
  // constant term for the LP relaxation part of the objective
  Vector objective_a_linear_term_;
  // linear term for the LP relaxation part of the objective
  Vector objective_b_linear_term_;
  // constant term for the LP relaxation part of the constraints
  Vector constraint_a_linear_term_;
  // linear term for the LP relaxation part of the constraints
  Vector constraint_b_linear_term_;
  AllkNN allknn_;
  bool is_infeasible_;
  
  ArrayList<index_t> rows_;
  ArrayList<index_t> columns_;
  ArrayList<double> values_;
  // lower bound for the optimization variable
  Matrix x_lower_bound_;
  // upper bound for the optimization variable
  Matrix x_upper_bound_;
  // soft lower bound of the relaxation
  double soft_lower_bound_;
  // tolerance for the gradient norm
  double grad_tolerance_;
  double sigma_;
};

class RelaxedNmfScaled {
 public:
  void Init(fx_module *module,
            ArrayList<index_t> &rows,
            ArrayList<index_t> &columns,
            ArrayList<double> &values,
            Matrix &x_lower_bound, // the initial lower bound for x (optimization variable)
            Matrix &x_upper_bound  // the initial upper bound for x (optimization variable)
           );
  void Destruct();
  // The following are required by LBFGS
  void ComputeGradient(Matrix &coordinates, Matrix *gradient);
  void ComputeObjective(Matrix &coordinates, double *objective);
  // This class implements a convex relaxation of the nmf objective
  // At some point we need to compute the original objective the non relaxed
  void ComputeNonRelaxedObjective(Matrix &coordinates, double *objective);
  void ComputeFeasibilityError(Matrix &coordinates, double *error);
  double ComputeLagrangian(Matrix &coordinates);
  void UpdateLagrangeMult(Matrix &coordinates);
  void Project(Matrix *coordinates);
  void set_sigma(double sigma); 
  void GiveInitMatrix(Matrix *init_data);
	bool IsDiverging(double objective); 
  bool IsOptimizationOver(Matrix &coordinates, Matrix &gradient, double step);
  bool IsIntermediateStepOver(Matrix &coordinates, Matrix &gradient, double step);
  
  // The following are required by the branch and bound
  double GetSoftLowerBound();
    
 private:
  fx_module *module_;
  // number of rows of the original matrix
  index_t num_of_rows_;
  // number of columns of the original matrix
  index_t num_of_columns_;
  // offset of the H matrix on the coordinate variable
  index_t h_offset_;
  index_t w_offset_;
  double values_sq_norm_;
  index_t new_dimension_;
  // constant term for the LP relaxation part of the dot products
  Vector a_linear_term_dot_prod_;
  // linear term for the LP relaxation part of the dot products
  Vector b_linear_term_dot_prod_;
  // constant term for the LP relaxation part for the linear terms
  Vector a_linear_term_lin_;
  Vector b_linear_term_lin_;
  ArrayList<index_t> rows_;
  ArrayList<index_t> columns_;
  ArrayList<double> values_;
  // lower bound for the optimization variable
  Matrix x_lower_bound_;
  // upper bound for the optimization variable
  Matrix x_upper_bound_;
  // soft lower bound of the relaxation
  double soft_lower_bound_;
  // tolerance for the gradient norm
  double grad_tolerance_;
  double previous_objective_;
  double scale_factor_;
  double epsilon_;
};

struct SolutionPack {
 public:
  SolutionPack() {
  }
  ~SolutionPack() {
    
  }
  double relaxed_minimum_;
  double non_relaxed_minimum_;
  Matrix solution_;
  std::pair<Matrix, Matrix> box_; 
};

template<typename SplitterClass>
class GopNmfEngine {
 public:
  
  typedef LBfgs<RelaxedNmf> LowerOptimizer; 
  typedef LBfgs<GeometricNmf> UpperOptimizer;
  void Init(fx_module *module, SplitterClass *splitter, Matrix &data_points);
  void ComputeGlobalOptimum();

 private:
  fx_module *module_;
  fx_module *l_bfgs_module_;
  fx_module *relaxed_nmf_module_;
  Matrix x_upper_bound_;
  Matrix x_lower_bound_;
  SplitterClass *splitter_;
  RelaxedNmf opt_fun_;
  double desired_global_optimum_gap_;
  double grad_tolerance_;
  std::multimap<double, SolutionPack> lower_solution_;
  SolutionPack upper_solution_;
  ArrayList<index_t> rows_;
  ArrayList<index_t> columns_;
  ArrayList<double> values_;
  index_t w_offset_;
  index_t h_offset_;
  double epsilon_;
  double scale_factor_;
  index_t num_of_rows_;
  index_t num_of_columns_;
  index_t new_dimension_;
  index_t soft_prunes_;
  index_t hard_prunes_;
  double soft_pruned_volume_;
  double hard_pruned_volume_;
  double total_volume_;
  index_t iteration_;

  void PreprocessData(Matrix &data_mat);
  double ComputeVolume(Matrix &lower_bound, Matrix &upper_bound);
  void ReportResults();
};


#include "gop_nmf_impl.h"

#endif
