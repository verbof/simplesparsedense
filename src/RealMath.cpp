#include "config.hpp"
#include "RealMath.hpp"
#include "timing.hpp"
#include "CSRdouble.hpp"
#include "ParDiSO.hpp"
#include "shared_var.h"
#include <cassert>
#include <cstring>


#define mesh(i,j,k) mesh[nx*ny*((k)-1) + nx*((j)-1)+(i)-1]
#define nzvals(i) nzvals[(i)-1]
#define rowind(i) rowind[(i)-1]
#define colptr(i) colptr[(i)-1]
#define xsol(i)   xsol[(i)-1]
#define rhs(i)    rhs[(i)-1]
#define diag(i)   diag[(i)-1]
#define diag2(i)  diag2[(i)-1]


void solveSystem(CSRdouble& A, double* X, double* B, int pardiso_mtype, int number_of_rhs)
{
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@ S O L V I N G     A    L I N E A R    S Y S T E M  @@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;

    cout << "*** G e n e r a t i n g    # " << number_of_rhs << "   r h s *** " << endl;

    // initialize pardiso and forward to it minimum number of necessary parameters
    /*int pardiso_message_level = 0;

    ParDiSO pardiso(pardiso_mtype, pardiso_message_level);*/


    pardiso_mtype = pardiso_mtype;

    // Numbers of processors, value of OMP_NUM_THREADS
    int number_of_processors = 1;
    char* var = getenv("OMP_NUM_THREADS");

    if(var != NULL)
        sscanf( var, "%d", &number_of_processors );
    else {
        printf("Set environment OMP_NUM_THREADS to 1. \n");
        exit(1);
    }

    pardiso_var.iparm[3]  = number_of_processors;
    pardiso_var.iparm[8]  = 0;


    timing secs;
    double initializationTime = 0.0;
    double factorizationTime  = 0.0;
    double solutionTime       = 0.0;



    //cout << "S Y M B O L I C     V O O D O O" << endl;

    secs.tick(initializationTime);
    pardiso_var.init(A, number_of_rhs);
    secs.tack(initializationTime);



    //cout << "L U                 F A C T O R I Z A T I O N" << endl;

    secs.tick(factorizationTime);
    pardiso_var.factorize(A);
    secs.tack(factorizationTime);


    //cout << "L U                 B A C K - S U B S T I T U T I O N" << endl;

    secs.tick(solutionTime);
    pardiso_var.solve(A, X, B);
    secs.tack(solutionTime);


    //errorReport(number_of_rhs, A, B, X);
    // writeSolution(number_of_rhs, A.nrows, X);

    if(iam==0){
    cout << "-------------------------------" << endl;
    cout << "T I M I N G         R E P O R T" << endl;
    cout << "-------------------------------" << endl;
    cout.setf(ios::floatfield, ios::scientific);
    cout.precision(4);
    cout << "Initialization phase: " << initializationTime*0.001 << " sec" << endl;
    cout << "Factorization  phase: " << factorizationTime*0.001 << " sec" << endl;
    cout << "Solution       phase: " << solutionTime*0.001 << " sec" << endl;}
}

void calculateSchurComplement(CSRdouble& A, int pardiso_mtype, CSRdouble& S)
{
    /*cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@ C A L C U L A T I N G     S C H U R - C O M P L M. @@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;*/


    // initialize pardiso and forward to it minimum number of necessary parameters
    /*int pardiso_message_level = 0;

    ParDiSO pardiso(pardiso_mtype, pardiso_message_level);*/


    pardiso_mtype = pardiso_mtype;

    // Numbers of processors, value of OMP_NUM_THREADS
    int number_of_processors = 1;
    char* var = getenv("OMP_NUM_THREADS");
    if (var != NULL)
        sscanf( var, "%d", &number_of_processors );

    pardiso_var.iparm[2]  = 2;
    pardiso_var.iparm[3]  = number_of_processors;
    pardiso_var.iparm[8]  = 0;
    pardiso_var.iparm[11] = 1;
    pardiso_var.iparm[13]  = 1;
    pardiso_var.iparm[28]  = 0;


    double schurTime  = 0.0;
    timing secs;

    //cout << "number of perturbed pivots = " << pardiso.iparm[14] << endl;

    secs.tick(schurTime);
    pardiso_var.makeSchurComplement(A, S);
    secs.tack(schurTime);

    cout << "Memory needed by ParDiSO for SCHUR COMPLEMENT: " << pardiso_var.memoryAllocated() << endl;

    //cout << "number of perturbed pivots = " << pardiso.iparm[14] << endl;
}

void errorReport(int number_of_rhs, CSRdouble& A, double* b, double* x)
{
    double* r = new double[number_of_rhs * A.nrows];


    for (int i = 0; i < number_of_rhs; i++)
    {
        double* b_i = b + i*A.nrows;
        double* x_i = x + i*A.nrows;
        double* r_i = r + i*A.nrows;

        A.residual(r_i, x_i, b_i);
    }


    double rnorm = 0.0;
    double normb = 0.0;

    for (int i = 0; i < number_of_rhs*A.nrows; i++)
    {
        rnorm += r[i]*r[i];
        normb += b[i]*b[i];
    }


    cout.setf(ios::scientific, ios::floatfield);
    cout.precision(16);

    cout << endl << endl;
    cout << "-----------------------------" << endl;
    cout << "R E S I D U A L     N O R M S" << endl;
    cout << "-----------------------------" << endl;
    cout << "||r_k||_2                = " << sqrt(rnorm) / number_of_rhs << endl;
    cout << "|| b ||_2                = " << sqrt(normb) << endl;
    cout << "||r_k||_2 / || b ||_2    = " << sqrt(rnorm) / sqrt(normb) << endl;
    cout << endl << endl;

    delete[] r;
}

void writeSolution(int number_of_solution_vectors, int n, double* X)
{
    cout.setf(ios::scientific, ios::floatfield);
    cout.precision(16);
    for (int i = 0; i < number_of_solution_vectors; i++)
    {
        double* x_i = X + i*n;

        for (int j = 0; j < n; j++)
        {
            cout << setw(60) << x_i[j] << endl;
        }
    }
    cout << endl;
}

void generateRhs(int number_of_rhs, int n, double* B)
{
    memset(B, 0, n*number_of_rhs*sizeof(double));

    for (int i = 0; i < number_of_rhs; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double value = std::rand() / (RAND_MAX*1.0);
            B[n*i + j] = value;
        }
    }
}

void solveManyRhsUsingSchurComplement(CSRdouble& A, int nrhs, int pardiso_mtype)
{
    CSRdouble Imat;   // identity
    CSRdouble Bmat;   // the CSR matrix of the rhs
    CSRdouble K;      // the augmented matrix
    CSRdouble S;      // the Schur-complement matrix



    double* B = new double[A.nrows*nrhs];

    timing secs;
    double generateRhsTime    = 0.0;
    double fillSymTime        = 0.0;
    double identityTime       = 0.0;
    double augmentedTime      = 0.0;
    double solutionTime       = 0.0;


    cout << "*** G e n e r a t i n g    # " << nrhs << " r h s *** " << endl;
    generateRhs(nrhs, A.nrows, B);
    cout << "generatingRhs  phase: " << setw(10) << generateRhsTime*0.001 << " sec" << endl;

    /*
      if (abs(pardiso_mtype) == 2)
      {
        secs.tick(fillSymTime);
        A.fillSymmetric();
        secs.tack(fillSymTime);
        pardiso_mtype = 1;
      }
    */

    cout << "*** G e n e r a t i n g    m a t r i x    B *** " << endl;
    secs.tick(generateRhsTime);
    makeRhsCSRdoubleMatrix(nrhs, A.nrows, B, Bmat);
    secs.tack(generateRhsTime);


    cout << "*** G e n e r a t i n g    m a t r i x    I *** " << endl;
    secs.tick(identityTime);
    makeIdentity(A.nrows, Imat);
    secs.tack(identityTime);
    cout << "IdentityTime   phase: " << setw(10) << identityTime*0.001      << " sec" << endl;



    cout << "***                                           [ A   B ] *** " << endl;
    cout << "*** G e n e r a t i n g    m a t r i x    K = [       ] *** " << endl;
    cout << "***                                           [ I   0 ] *** " << endl;
    secs.tick(augmentedTime);
    createAugmentedMatrix(A, Bmat, Imat, K);
    secs.tack(augmentedTime);
    K.writeToFile("AugmentedMatrixA_B_I_ZERO_NoRhsEqual2.csr");
    cout << "augmentedTime  phase: " << setw(10) << augmentedTime*0.001     << " sec" << endl;



    S.nrows = A.nrows;
    S.ncols = A.ncols;



    cout << "                                                 -1         " << endl;
    cout << "*** G e n e r a t i n g    S c h u r      S = - A   B   *** " << endl;
    secs.tick(solutionTime);
    calculateSchurComplement(K, 11, S);
    secs.tack(solutionTime);
    cout << "Solution       phase: " << setw(10) << solutionTime*0.001      << " sec" << endl;

    cout << endl << endl;
    cout << "S.nonzeros = " << S.nonzeros << endl;
    cout << "S.nonzeros = " << nrhs*A.nrows << endl;
    cout << endl << "*** F I N I T O ***" << endl;



    double* Xs = new double[S.nonzeros];
    assert(S.nonzeros >= nrhs*A.nrows);
    for (int j = 0; j < A.nrows; j++)
    {
        for (int i = 0; i < nrhs; i++)
        {
            Xs[i*A.nrows + j] = S.pData[i + j*A.nrows];
        }
    }



    /*
       cout.setf(ios::scientific, ios::floatfield);
       cout.precision(16);
       for (int j = 0; j < A.nrows; j++)
       {
       for (int i = 0; i < nrhs; i++)
       {
       cout << setw(26) << B[i*A.nrows + j];
       cout << setw(26) << Xs[i*A.nrows + j];
       }
       cout << endl;
       }
       */

    errorReport(nrhs, A, B, Xs);

    cout << "-------------------------------" << endl;
    cout << "T I M I N G         R E P O R T" << endl;
    cout << "-------------------------------" << endl;
    cout.setf(ios::floatfield, ios::scientific);
    cout.precision(4);
    cout << "generatingRhs  phase: " << setw(10) << generateRhsTime*0.001   << " sec" << endl;
    cout << "fillSymTime    phase: " << setw(10) << fillSymTime*0.001       << " sec" << endl;
    cout << "IdentityTime   phase: " << setw(10) << identityTime*0.001      << " sec" << endl;
    cout << "augmentedTime  phase: " << setw(10) << augmentedTime*0.001     << " sec" << endl;
    cout << "Solution       phase: " << setw(10) << solutionTime*0.001      << " sec" << endl;

    cout << endl << endl;

    double* X = new double[nrhs*A.nrows];

    solveSystem(A, X, B, pardiso_mtype, nrhs);

    norm(nrhs*A.nrows, X, Xs);

    /*
       cout.setf(ios::scientific, ios::floatfield);
       cout.precision(16);
       for (int j = 0; j < A.nrows; j++)
       {
       for (int i = 0; i < nrhs; i++)
       {
       cout << setw(26) << X[i*A.nrows + j];
       cout << setw(26) << Xs[i*A.nrows + j];
       }
       cout << endl;
       }
    */


    /*
      char filename[1024];
      sprintf(filename, "K_rhs%d.csr", nrhs);
      K.writeToFile(filename);
      sprintf(filename, "S_rhs%d.csr", nrhs);
      S.writeToFile(filename);
      sprintf(filename, "B_rhs%d.csr", nrhs);
      Bmat.writeToFile(filename);

    */

    delete[] Xs;
    delete[] X;
    delete[] B;
}

void createAugmentedMatrix(CSRdouble& A, CSRdouble& B, CSRdouble& C, // input
                           CSRdouble& K)  // output
{
    int nrows    = A.nrows + C.nrows;
    int ncols    = A.ncols + B.ncols;
    int nonzeros = A.nonzeros + B.nonzeros + C.nonzeros
                   + B.ncols; // for the 0 block we still need to keep 1 entry for each row

    if (A.matrixType == SYMMETRIC)
    {
        nonzeros += A.nonzeros - A.nrows;
    }

    vector<vector<int> >    vvcols(nrows);
    vector<vector<double> > vvdata(nrows);

    if (A.nrows != B.nrows)
    {
        cout << "error in createSchurComplementStructure: A.nrows != B.nrows, " << A.nrows << "!=" << B.nrows << endl;
    }

    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int j        = A.pCols[index];
            double a_ij  = A.pData[index];

            vvcols[i].push_back(j);
            vvdata[i].push_back(a_ij);

            if (A.matrixType == SYMMETRIC)
            {
                if (i != j)
                {
                    vvcols[j].push_back(i);
                    vvdata[j].push_back(a_ij);
                }
            }
        }

        // push ith row of B
        for (int index = B.pRows[i]; index < B.pRows[i+1]; index++)
        {
            int j        = B.pCols[index];
            double b_ij  = B.pData[index];

            vvcols[i].push_back(A.ncols + j);
            vvdata[i].push_back(b_ij);
        }
    }



    for (int i = 0; i < C.nrows; i++)
    {
        // push ith row of C
        for (int index = C.pRows[i]; index < C.pRows[i+1]; index++)
        {
            int j        = C.pCols[index];
            double c_ij  = C.pData[index];

            vvcols[A.nrows + i].push_back(j);
            vvdata[A.nrows + i].push_back(c_ij);
        }

        // 0 block
        vvcols[A.nrows + i].push_back(A.nrows + i);
        vvdata[A.nrows + i].push_back(0.0);
    }


    int* ia   = new int[nrows + 1];
    int* ja   = new int[nonzeros];
    double* a = new double[nonzeros];

    ia[0] = 0;
    for (int i = 0; i < nrows; i++)
    {
        ia[i+1] = ia[i] + vvcols[i].size();

        for (int index = ia[i]; index < ia[i+1]; index++)
        {
            ja[index]    = vvcols[i][index - ia[i]];
            a[index]     = vvdata[i][index - ia[i]];
        }
    }


    K.make(nrows, ncols, nonzeros, ia, ja, a);
    K.sortColumns();
    // K.writeToFile("K.csr");
}

void create2x2SymBlockMatrix(CSRdouble& A, CSRdouble& B, CSRdouble& T, // input
                             CSRdouble& C)  // output
{
    assert(A.nrows==B.nrows);
    assert(A.ncols==B.nrows);
    assert(T.ncols==B.ncols);
    assert(T.nrows==B.ncols);

    int nrows    = A.nrows + T.nrows;
    int ncols    = A.ncols + T.ncols;
    int nonzeros = A.nonzeros + B.nonzeros + T.nonzeros;

    int* ic   = new int[nrows + 1];
    int* jc   = new int[nonzeros];
    double* c = new double[nonzeros];

    int nonzero_counter = 0;
    ic[0] = nonzero_counter;
    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int& j              = A.pCols[index];
            if (j>=i)
            {
                double& a_ij        = A.pData[index];

                c[nonzero_counter] = a_ij;
                jc[nonzero_counter] = j;

                nonzero_counter++;
            }
        }

        // push ith row of B
        for (int index = B.pRows[i]; index < B.pRows[i+1]; index++)
        {
            int& j              = B.pCols[index];
            double& b_ij        = B.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[i+1] = nonzero_counter;
    }

    for (int i = 0; i < T.nrows; i++)
    {
        // push ith row of T
        for (int index = T.pRows[i]; index < T.pRows[i+1]; index++)
        {
            int& j              = T.pCols[index];
            double& t_ij        = T.pData[index];

            c[nonzero_counter] = t_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[A.nrows+i+1] = nonzero_counter;
    }
    if (nonzero_counter != nonzeros)
        cout << "Nonzeroes do not match, nonzero_counter= " << nonzero_counter << "; nonzeros= " << nonzeros <<endl;


    C.make(nrows, ncols, nonzero_counter, ic, jc, c);
    C.sortColumns();
    // C.writeToFile("C.csr");
}

void create2x2BlockMatrix(CSRdouble& A, CSRdouble& B, CSRdouble& C, CSRdouble& D, // input
                             CSRdouble& W)  // output
{
    assert(A.nrows==B.nrows);
    assert(A.ncols==C.ncols);
    assert(D.ncols==B.ncols);
    assert(D.nrows==C.nrows);

    int nrows    = A.nrows + D.nrows;
    int ncols    = A.ncols + D.ncols;
    int nonzeros = A.nonzeros + B.nonzeros + C.nonzeros + D.nonzeros;

    int* ic   = new int[nrows + 1];
    int* jc   = new int[nonzeros];
    double* c = new double[nonzeros];

    int nonzero_counter = 0;
    ic[0] = nonzero_counter;
    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int& j              = A.pCols[index];
            double& a_ij        = A.pData[index];

                c[nonzero_counter] = a_ij;
                jc[nonzero_counter] = j;

                nonzero_counter++;
        }

        // push ith row of B
        for (int index = B.pRows[i]; index < B.pRows[i+1]; index++)
        {
            int& j              = B.pCols[index];
            double& b_ij        = B.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[i+1] = nonzero_counter;
    }

    for (int i = 0; i < D.nrows; i++)
    {
      // push ith row of C
        for (int index = C.pRows[i]; index < C.pRows[i+1]; index++)
        {
            int& j              = C.pCols[index];
            double& b_ij        = C.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = j;

            nonzero_counter++;
        }
        // push ith row of T
        for (int index = D.pRows[i]; index < D.pRows[i+1]; index++)
        {
            int& j              = D.pCols[index];
            double& t_ij        = D.pData[index];

            c[nonzero_counter] = t_ij;
            jc[nonzero_counter] = C.ncols + j;

            nonzero_counter++;
        }

        ic[A.nrows+i+1] = nonzero_counter;
    }
    if (nonzero_counter != nonzeros)
        cout << "Nonzeroes do not match, nonzero_counter= " << nonzero_counter << "; nonzeros= " << nonzeros <<endl;


    W.make(nrows, ncols, nonzeros, ic, jc, c);
    W.sortColumns();
    // C.writeToFile("C.csr");
}

void create1x2BlockMatrix(CSRdouble& A, CSRdouble& B, // input
                          CSRdouble& C)  // output
{
    cout << "***  G e n e r a t i n g    m a t r i x   C = [ A      B ] *** " << endl;

    int nrows    = A.nrows;
    int ncols    = A.ncols + B.ncols;
    int nonzeros = A.nonzeros + B.nonzeros;

    int* ic   = new int[nrows + 1];
    int* jc   = new int[nonzeros];
    double* c = new double[nonzeros];

    int nonzero_counter = 0;
    ic[0] = nonzero_counter;
    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int& j              = A.pCols[index];
            double& a_ij        = A.pData[index];

            c[nonzero_counter] = a_ij;
            jc[nonzero_counter] = j;

            nonzero_counter++;

        }

        // push ith row of B
        for (int index = B.pRows[i]; index < B.pRows[i+1]; index++)
        {
            int& j              = B.pCols[index];
            double& b_ij        = B.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[i+1] = nonzero_counter;
    }


    if (nonzero_counter != nonzeros)
        cout << "Nonzeroes do not match, nonzero_counter= " << nonzero_counter << "; nonzeros= " << nonzeros <<endl;


    C.make(nrows, ncols, nonzeros, ic, jc, c);
    C.sortColumns();
    // C.writeToFile("C.csr");
}

void create2x2SymBlockMatrix_denseT(CSRdouble& A, CSRdouble& B, double* T, // input
                                    CSRdouble& C)  // output
{
    cout << "***                                           [ A      B ] *** " << endl;
    cout << "***                                           [          ] *** " << endl;
    cout << "*** G e n e r a t i n g    m a t r i x    C = [          ] *** " << endl;
    cout << "***                                           [  t       ] *** " << endl;
    cout << "***                                           [ B      T ] *** " << endl;
    int nrows    = A.nrows + B.ncols;
    int ncols    = A.ncols + B.ncols;
    int nonzeros = A.nonzeros + B.nonzeros + (B.ncols * B.ncols - B.ncols)/2 + B.ncols;

    int i,j;

    int* ic   = new int[nrows + 1];
    int* jc   = new int[nonzeros];
    double* c = new double[nonzeros];

    int nonzero_counter = 0;
    ic[0] = nonzero_counter;
    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int& j              = A.pCols[index];
            if (j>=i)
            {
                double& a_ij        = A.pData[index];

                c[nonzero_counter] = a_ij;
                jc[nonzero_counter] = j;

                nonzero_counter++;
            }
        }

        // push ith row of B
        for (int index = B.pRows[i]; index < B.pRows[i+1]; index++)
        {
            int& j              = B.pCols[index];
            double& b_ij        = B.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[i+1] = nonzero_counter;
    }

    for (i = 0; i < B.ncols; i++)
    {
        // push ith row of T
        for (j = i; j < B.ncols; j++)
        {
            double& t_ij        = T[j*B.ncols + i];

            c[nonzero_counter] = t_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;

        }

        ic[A.nrows+i+1] = nonzero_counter;
    }
    if (nonzero_counter != nonzeros)
        cout << "Nonzeroes do not match, nonzero_counter= " << nonzero_counter << "; nonzeros= " << nonzeros <<endl;
    C.make(nrows, ncols, nonzeros, ic, jc, c);
    C.sortColumns();
    // C.writeToFile("C.csr");
}

void create2x2BlockMatrix_denseT_lldT(CSRdouble& A, CSRdouble& BT_i, CSRdouble& B_j, double* T, int lld_T,// input
                                      CSRdouble& C)  // output
{

    //A.fillSymmetric();
    int nrows    = A.nrows + BT_i.nrows;
    int ncols    = A.ncols + B_j.ncols;
    int nonzeros = A.nonzeros + BT_i.nonzeros + B_j.nonzeros + BT_i.nrows * B_j.ncols;

    int i,j;

    int* ic   = new int[nrows + 1];
    int* jc   = new int[nonzeros];
    double* c = new double[nonzeros];

    int nonzero_counter = 0;
    ic[0] = nonzero_counter;
    for (int i = 0; i < A.nrows; i++)
    {
        // push ith row of A
        for (int index = A.pRows[i]; index < A.pRows[i+1]; index++)
        {
            int& j              = A.pCols[index];

            double& a_ij        = A.pData[index];

            c[nonzero_counter] = a_ij;
            jc[nonzero_counter] = j;

            nonzero_counter++;
        }

        // push ith row of B_j
        for (int index = B_j.pRows[i]; index < B_j.pRows[i+1]; index++)
        {
            int& j              = B_j.pCols[index];
            double& b_ij        = B_j.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;
        }

        ic[i+1] = nonzero_counter;
    }

    for (i = 0; i < BT_i.nrows; i++)
    {
        //push ith row of B_i^T
        for (int index = BT_i.pRows[i]; index < BT_i.pRows[i+1]; index++)
        {
            int& j              = BT_i.pCols[index];
            double& b_ij        = BT_i.pData[index];

            c[nonzero_counter] = b_ij;
            jc[nonzero_counter] = j;

            nonzero_counter++;
        }

        // push ith row of T
        for (j = 0; j < B_j.ncols; j++)
        {
            double& t_ij        = T[j*lld_T + i];

            c[nonzero_counter] = t_ij;
            jc[nonzero_counter] = A.ncols + j;

            nonzero_counter++;

        }

        ic[A.nrows+i+1] = nonzero_counter;
    }
    if (nonzero_counter != nonzeros)
        cout << "Nonzeroes do not match in processor " << iam << ", nonzero_counter= " << nonzero_counter << "; nonzeros= " << nonzeros <<endl;
    C.make(nrows, ncols, nonzeros, ic, jc, c);
    C.sortColumns();
    // C.writeToFile("C.csr");
}

void makeRhsCSRdoubleMatrix(int number_of_rhs, int n, double* B, CSRdouble& Bmat)
{
    int  nonzeros = 0;

    vector<int> vcols;
    vector<double> vdata;

    int* prows = new int[n+1];
    prows[0]   = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < number_of_rhs; j++)
        {
            int ij       = j*n + i;
            double& b_ij = B[ij];

            if (b_ij != 0)
            {
                vdata.push_back(b_ij);
                vcols.push_back(j);
                nonzeros++;
            }
        }
        /*
           if (i >= number_of_rhs)
           {
           vdata.push_back(1e-15);
           vcols.push_back(i);
           nonzeros++;
           }
           */
        prows[i+1]  = nonzeros;
    }

    double* pdata = new double[nonzeros];
    int*    pcols = new int[nonzeros];

    memcpy(pcols, &vcols[0], nonzeros*sizeof(int));
    memcpy(pdata, &vdata[0], nonzeros*sizeof(double));

    Bmat.make(n, n, nonzeros, prows, pcols, pdata);
}

void makeIdentity(int n, CSRdouble& I)
{
    int*    prows = new int[n+1];
    int*    pcols = new int[n];
    double* pdata = new double[n];
    prows[0]      = 0;

    for (int i = 0; i < n; i++)
    {
        prows[i+1]  = prows[i] + 1;
        pcols[i]    = i;
        pdata[i]    = 1.0;
    }

    I.make(n, n, n, prows, pcols, pdata);
}


void makeDiagonal(int n, double* val, CSRdouble& D)
{
    int*    prows = new int[n+1];
    int*    pcols = new int[n];
    double* pdata = new double[n];
    prows[0]      = 0;

    for (int i = 0; i < n; i++)
    {
        prows[i+1]  = prows[i] + 1;
        pcols[i]    = i;
        pdata[i]    = val[i];
    }

    D.make(n, n, n, prows, pcols, pdata);
}



void makeDiagonalPerturbD(int n, double val, double pert, CSRdouble& DP)
{
    int*    prows = new int[n+2];
    int*    pcols = new int[n+1];
    double* pdata = new double[n+1];

    prows[0] = 0;
    prows[1] = 2;
    pcols[0] = 0;
    pcols[1] = 1;
    pdata[0] = val;
    pdata[1] = pert;

    for (int i = 2; i < n+1; i++)
    {
        prows[i] = prows[i-1] + 1;
        pcols[i] = i-1;
        pdata[i] = val;
    }

    DP.make(n, n, n+1, prows, pcols, pdata);

}



void makeDiagonalPerturbV(int n, double* val, double pert, CSRdouble& DP)
{
    int*    prows = new int[n+2];
    int*    pcols = new int[n+1];
    double* pdata = new double[n+1];

    prows[0] = 0;
    prows[1] = 2;
    pcols[0] = 0;
    pcols[1] = 1;
    pdata[0] = val[0];
    pdata[1] = pert;

    for (int i = 2; i < n+1; i++)
    {
        prows[i] = prows[i-1] + 1;
        pcols[i] = i-1;
        pdata[i] = val[i-1];
    }

    DP.make(n, n, n+1, prows, pcols, pdata);

}


void genZeros(int m, int n, double* zeros)
{

    for(size_t j = 0; j < m*n; j++)
    {
        zeros[j] = 0.0;
    }

}


void genAlmostOnes(int m, int n, double value, double* ones)
{

    for (size_t j = 0; j < m; j++)
    {
        for (size_t k = 0; k < n; k++)
        {
            ones[j+k*(size_t)m] = 0.0 + value*(j+k+1);
        }
    }

}


void genOnes(int m, int n, double value, double* ones)
{

    for (size_t j = 0; j < m*n; j++)
    {
        ones[j] = 0.0 + value;
    }

}


void makeOnes(int m, int n, double value, CSRdouble& E)
{
    int         N = m*n;
    int*    prows = new int[N+1];
    int*    pcols = new int[N];
    double* pdata = new double[N];
    prows[0]      = 0;

    for (size_t i = 0; i < m; i++)
    {
        prows[i+1] = prows[i] + n;

        for (size_t k = 0; k < n; k++)
        {
            pcols[i*(size_t)n + k] = k;
            pdata[i*(size_t)n + k] = 0.0 + value;
        }
    }

    E.make(m, n, N, prows, pcols, pdata);
}


void genDiagonalV(int n, double* value, double* diag)
{
    for (size_t k = 0; k < n; k++)
    {
        diag[k*n + k] = value[k];
    }

}

void genDiagonalD(int n, double d, double* diag)
{
    for (size_t k = 0; k < n; k++)
    {
        diag[k*n + k] = 0.0 + d;
    }

}

void genTriDiag(int n, double* tri)
{
    for (int k = 0; k < n; k++)
    {
        tri[k*n + k] = 2.0;
    }

    for (int k = 0; k < n-1; k++)
    {
        tri[k*n + k+1] = -1.0;
    }

    for (int k = 1; k < n; k++)
    {
        tri[k*n + k-1] = -1.0;
    }

}




void shiftIndices(CSRdouble& A, int value)
{
  int i;
  for (i = 0; i <= A.nrows; i++)
  {
    A.pRows[i] += value;
  }

  for (i = 0; i < A.nonzeros; i++)
  {
    A.pCols[i] += value;
  }    
}

void make3DLaplace(int nx, int ny, int nz, CSRdouble& L)
{

   int i, j, k, nnodes,nedges, nnz;
   int count, node;
   int* mesh;
   int* rowind;
   int* colptr;
   double* nzvals;
   int nnz_full;
   double dval;


   nnodes = nx*ny*nz;
   cout << "Generating a " << nnodes << "-by-" << nnodes << " Laplacian matrix..." << endl;

   mesh = (int*) calloc(nnodes, sizeof(int));

   for (i = 0; i<nnodes; i++) mesh[i]=i+1;
   
   /* first pass to count the number of edges */
   /* Dirichlet BC */ 
   nedges = 0; 
  
   for (k = 1; k <= nz; k++) {
     for (j = 1; j <= ny; j++) {
       for (i = 1; i <= nx; i++) {
       if (k < nz) nedges++;
       if (k > 1)  nedges++;
       if (j < ny) nedges++;
       if (j > 1)  nedges++;
       if (i < nx) nedges++;
       if (i > 1)  nedges++;   
     }
   }
   }

   /* print the matrix dimension and number of nonzeros */
   nnz = nedges/2 + nnodes;
   //printf("%d  %d\n", nnodes, nnz);
   nnz_full = nnz;
   
   colptr  = (int*)malloc((nnodes+1)*sizeof(int));
   rowind  = (int*)malloc(nnz_full*sizeof(int));
   nzvals  = (double*)malloc(nnz_full*sizeof(double));

   colptr(1) = 1;
   count = 0;
   node  = 0;

  
   dval = 20.0;
   //printf(" Laplacian with Dirichlet boundary condition\n");

   for (k = 1; k <= nz; k++) {
     for (j = 1; j <= ny; j++) {
        for (i = 1; i <= nx; i++) {
	 /* diagonal */
         dval = nx*ny*nz + (i + j + k) / 100 ;
         //if (printa)
            //printf("%d %d  %8.2e\n", mesh(i,j,k), mesh(i,j,k), dval); 

         rowind[count] = mesh(i,j,k); 
         nzvals[count] = dval + node; /* no pivots */
         count++;

         /* lower */
         if (i < nx) {
            //if (printa) 
	       //printf("%d %d -1.0\n", mesh(i+1,j,k), mesh(i,j,k));

            rowind[count] = mesh(i+1,j,k);
            dval = -1.0;
            dval = (i + j + k) / 100 ;
            nzvals[count] = dval;
            count++;
         }


         /* right */
         if (j < ny) {
            //if (printa) 
	       //printf("%d %d -1.0\n", mesh(i,j+1,k), mesh(i,j,k)); 

            rowind[count] = mesh(i,j+1,k);
            dval = -1.0;
            dval = (i + j + k) / 100 ;
            nzvals[count] = dval;
            count++;
         }       

         /* lower */
         if (k < nz) {
            //if (printa) 
	       //printf("%d %d -1.0\n", mesh(i,j,k+1), mesh(i,j,k));

            rowind[count] = mesh(i,j,k+1);
            dval = -1.0;
            nzvals[count] = dval;
            count++; 
         }

         node++;
         colptr(node+1) = count+1; 
         
      } 
   }
   }
   if (count != nnz) {
       printf(" is this correct? count = %d, nnz = %d\n", count, nnz);  
       return;
   }

    //rowind = ja, nzvals = a  nnodes colptr(nnodes) colptr(j) = ia, 


   L.allocate(nnodes, nnodes, colptr(nnodes));
   L.make2(nnodes, nnodes, colptr(nnodes), colptr, rowind, nzvals);

   /*
   if (printa) 
   { 
	printf("%d \n", nnodes );
	printf("%d \n", nnodes );
	printf("%d \n", colptr(nnodes) );
        for (j = 1; j <= nnodes+1; j++) 
		printf("%d \n", colptr(j)-1 );
        for (j = 1; j <= colptr(nnodes); j++) 
		printf("%d \n", rowind(j)-1 );
        for (j = 1; j <= colptr(nnodes); j++)
		printf("%e \n", nzvals(j) );
   }
   */

   free(mesh);
   free(colptr); 
   free(rowind); 
   free(nzvals); 

   cout << "- Done!" << endl;

}


void gen3DLaplace(int nx, int ny, int nz, double* lap)
{
   CSRdouble L;

   make3DLaplace(nx, ny, nz, L);

   CSR2dense(L, lap);
}



void norm(int n, double* x, double* y)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum += (x[i]-y[i]) * (x[i]-y[i]);
    }

    cout << std::scientific << std::setprecision(16) << "error between solutions is: " << sqrt(sum) << endl;
}


