#include "clp_wrapper.h"
#include "constraint.h"
#include "array.h"

#include <stdlib.h>

const char* Clp_Version()
{
    return "0";
}

int Clp_VersionMajor()
{
    return 0;
}

int Clp_VersionMinor()
{
    return 0;
}

int Clp_VersionRelease()
{
    return 0;
}

typedef struct Clp_Wrapper {
    enum {
        None = 0,
        Final = 1,
        Factorizations = 2,
        PlusABitMore = 3,
        Verbose = 4
    } log_level;
    enum {
        Maximize = -1,
        Ignore = 0,
        Minimize = 1
    } direction;
    enum {
        Unknown           = -1,
        Optimal           = 0,
        PrimalInfeasible  = 1,
        DualInfeasible    = 2,
        Stopped           = 3,
        Errors            = 4,
        UserStopped       = 5
    } status;
    array rows_upper;
    array rows_lower;
    array cols_upper;
    array cols_lower;
} Clp_Wrapper;

Clp_Simplex *Clp_newModel()
{
    Clp_Wrapper *model = malloc(sizeof(Clp_Wrapper));
    model->status = Unknown;
    Clp_setLogLevel(model, Verbose);
    Clp_setOptimizationDirection(model, Maximize);
    model->rows_upper = make_array(4, standard_constraint);
    model->rows_lower = make_array(4, standard_constraint);
    model->cols_upper = make_array(4, standard_constraint);
    model->cols_lower = make_array(4, standard_constraint);
    return model;
}

void Clp_deleteModel(Clp_Simplex *model)
{
    Clp_Wrapper *m = model;
    free_array(m->rows_upper);
    free_array(m->rows_lower);
    free_array(m->cols_upper);
    free_array(m->cols_lower);
    free(model);
}


int Clp_readMps(Clp_Simplex *model, const char *fn, int keep_names, int ignore_errors)
{
    return 0;
}

void Clp_addRows(Clp_Simplex *model, int number,
                 double *row_lower, double *row_upper, int *row_starts,
                 int *columns, double *elements)
{
    Clp_Wrapper *m = model;
    int i = 0;
    for(; i < number; ++i) {
        double as[2] = {0, 0};
        if(row_starts != NULL && columns != NULL && elements != NULL) {
            int j = 0;
            for(j = row_starts[i]; j < row_starts[i+1]; ++j) {
                if(columns[j] >= 0 && columns[j] <= 1) {
                    as[columns[j]] = elements[j];
                }
            }
        }
        {
            standard_constraint *row = grow(&m->rows_upper, standard_constraint);
            row->a1 = as[0];
            row->a2 = as[1];
            row->b  = row_upper[i];
        }
        {
            standard_constraint *row = grow(&m->rows_lower, standard_constraint);
            row->a1 = -as[0];
            row->a2 = -as[1];
            row->b  = row_lower[i];
        }
    }
}

void Clp_addColumns(Clp_Simplex *model, int number,
                    double *column_lower, double *column_upper,
                    double *objective, int *column_starts,
                    int *rows, double *elements)
{
    Clp_Wrapper *m = model;
    int i = 0;
    unsigned long col_len = m->cols_upper.length;
    assert(number + col_len <= 2); /* Can only handle 2D LP. */
    for(; i < number; ++i) {
        if(column_starts != NULL && rows != NULL && elements != NULL) {
            int j = 0;
            for(j = column_starts[i]; j < column_starts[i+1]; ++j) {
                if(rows[j] >= 0 && rows[j] <= m->rows_upper.length) {
                    standard_constraint *row = index(m->rows_upper, rows[j], standard_constraint);
                    *(col_len + i == 0 ? &row->a1 : &row->a2) = elements[j];
                }
            }
        }
        {
            standard_constraint *col = grow(&m->cols_upper, standard_constraint);
            *(col_len + i == 0 ? &col->a1 : &col->a2) = objective[i];
            col->b  = column_upper[i];
        }
        {
            standard_constraint *col = grow(&m->cols_lower, standard_constraint);
            *(col_len + i == 0 ? &col->a1 : &col->a2) = -objective[i];
            col->b  = column_lower[i];
        }
    }
}

void Clp_setOptimizationDirection(Clp_Simplex *model, double value)
{
    ((Clp_Wrapper*)model)->direction = value;
}

void Clp_setLogLevel(Clp_Simplex *model, int value)
{
    ((Clp_Wrapper*)model)->log_level = value;
}

int Clp_initialSolve(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status;
}


int Clp_getNumRows(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->rows_upper.length;
}

int Clp_getNumCols(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->cols_upper.length;
}


int Clp_isAbandoned(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == Errors;
}

int Clp_isProvenOptimal(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == Optimal;
}

int Clp_isProvenPrimalInfeasible(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == PrimalInfeasible;
}

int Clp_isProvenDualInfeasible(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == DualInfeasible;
}

int Clp_isPrimalObjectiveLimitReached(Clp_Simplex *model)
{
    return 0;
}

int Clp_isDualObjectiveLimitReached(Clp_Simplex *model)
{
    return 0;
}

int Clp_isIterationLimitReached(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == Stopped;
}


double *Clp_getRowActivity(Clp_Simplex *model)
{
    return 0;
}

double *Clp_getColSolution(Clp_Simplex *model)
{
    return 0;
}
