
// floop9 generated by makeloops.py Wed Jun 29 17:36:24 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define floop9_f77 floop9_f77_
 #define floop9_f77overhead floop9_f77overhead_
 #define floop9_f90 floop9_f90_
 #define floop9_f90overhead floop9_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define floop9_f77 floop9_f77__
 #define floop9_f77overhead floop9_f77overhead__
 #define floop9_f90 floop9_f90__
 #define floop9_f90overhead floop9_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define floop9_f77 FLOOP9_F77
 #define floop9_f77overhead FLOOP9_F77OVERHEAD
 #define floop9_f90 FLOOP9_F90
 #define floop9_f90overhead FLOOP9_F90OVERHEAD
#endif

extern "C" {
  void floop9_f77(const int& N, float* a, float* x);
  void floop9_f77overhead(const int& N, float* a, float* x);
  void floop9_f90(const int& N, float* a, float* x);
  void floop9_f90overhead(const int& N, float* a, float* x);

}

void VectorVersion(BenchmarkExt<int>& bench);
void ArrayVersion(BenchmarkExt<int>& bench);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench);
void ArrayVersion_index(BenchmarkExt<int>& bench);
void doTinyVectorVersion(BenchmarkExt<int>& bench);
void F77Version(BenchmarkExt<int>& bench);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench);
#endif

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;
const bool runvector=false; // no point as long as Vector is Array<1>

int main()
{
    int numBenchmarks = 6;
    if (runvector) numBenchmarks++;
#ifdef BENCHMARK_VALARRAY
    numBenchmarks++;
#endif
#ifdef FORTRAN_90
    numBenchmarks++;
#endif

    BenchmarkExt<int> bench("floop9: $x = $x+$a", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 1 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();



    ArrayVersion(bench);
    ArrayVersion_unaligned(bench);
    ArrayVersion_misaligned(bench);
    ArrayVersion_index(bench);
    doTinyVectorVersion(bench);
    F77Version(bench);
#ifdef FORTRAN_90
    F90Version(bench);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench);
#endif

    if(runvector)
      VectorVersion(bench);

    bench.endBenchmarking();

    bench.saveMatlabGraph("floop9.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<float> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<float> x(N);
        initializeRandomDouble(x.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion_index(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x(tensor::i)+a(tensor::i);;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> afill(N+1);
    Array<float,1> a(afill(Range(1,N)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> xfill(N+1);
    Array<float,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> afill(N+2);
    Array<float,1> a(afill(Range(0,N+0-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> xfill(N+2);
    Array<float,1> x(xfill(Range(1,N+1-1)));
    initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench)
{
        cout << bench.currentImplementation() << ": N = " << N << endl;

        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<float,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<float,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench)
{
  bench.beginImplementation("TinyVector<T>");
  // can't run tinyvector with full length because meta-unrolling
  // kills compiler...
  int N=Nmax;
  while(N> 1<<tvNmax) {
   bench.getParameter();
   bench.getIterations();
   bench.skip();
   N>>=1;
  }

  TinyVectorVersion< 1<<tvNmax >(bench);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<float> a(N);
        initializeRandomDouble(a, N);
        valarray<float> x(N);
        initializeRandomDouble(x, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* x = new float[N];
        initializeRandomDouble(x, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop9_f77(N, a, x);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop9_f77overhead(N, a, x);

        bench.stopOverhead();

        delete [] a;
        delete [] x;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* x = new float[N];
        initializeRandomDouble(x, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop9_f90(N, a, x);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop9_f90overhead(N, a, x);

        bench.stopOverhead();
        delete [] a;
        delete [] x;

    }

    bench.endImplementation();
}
#endif

