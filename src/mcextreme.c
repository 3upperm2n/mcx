/*******************************************************************************
 **
 **  Monte Carlo eXtreme - GPU accelerated Monte Carlo Photon Migration
 **  
 **  Author     : Qianqian Fang
 **  Email      : <q.fang at neu.edu>
 **  Institution: Department of Bioengineering, Northeastern University
 **  Address    : 360 Huntington Ave, Boston, MA 02115
 **  Homepage   : http://nmr.mgh.harvard.edu/~fangq/
 **
 **  MCX Web    : http://mcx.sourceforge.net
 **
 **  License    : GNU General Public License version 3 (GPLv3), see LICENSE.txt
 **
 *******************************************************************************/

#include <stdio.h>
#include "tictoc.h"
#include "mcx_utils.h"
#include "mcx_core.h"
#ifdef _OPENMP
#include <omp.h>
#endif

//#include <cuda_runtime.h>
//#include <helper_cuda.h>
//#include <helper_functions.h>

int main (int argc, char *argv[]) {
	Config  mcxconfig;
	GPUInfo *gpuinfo=NULL;
	unsigned int activedev=0;

	mcx_initcfg(&mcxconfig);

	// parse command line options to initialize the configurations
	mcx_parsecmd(argc,argv,&mcxconfig);

	// identify gpu number and set one gpu active
	if(!(activedev=mcx_list_gpu(&mcxconfig,&gpuinfo))){
		mcx_error(-1,"No GPU device found\n",__FILE__,__LINE__);
	}

#ifdef _OPENMP
	omp_set_num_threads(activedev);
#pragma omp parallel
	{
#endif

		// this launches the MC simulation


		//int num_streams = 2;
		//cudaStream_t *streams = (cudaStream_t *) malloc(num_streams * sizeof(cudaStream_t));

		//for (int i = 0; i < num_streams; i++) {
		//	cudaStreamCreate(&(streams[i]));
		//}

		mcx_run_simulation(&mcxconfig,gpuinfo);

		//mcx_run_simulation(&mcxconfig,gpuinfo, streams, 0);
		//mcx_run_simulation(&mcxconfig,gpuinfo, streams, 1);


		//for (int i = 0; i < num_streams; i++) {
		//	cudaStreamDestroy(streams[i]);
		//}


#ifdef _OPENMP
	}
#endif

	// clean up the allocated memory in config and gpuinfo
	mcx_cleargpuinfo(&gpuinfo);
	mcx_clearcfg(&mcxconfig);
	return 0;
}
