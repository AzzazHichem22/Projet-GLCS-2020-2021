// standard C++ library headers
#include <chrono>
#include <iostream>
#include <thread>
#include <hdf5.h>
// the implemented class (last)
#include "screenprinter.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void ScreenPrinter::simulation_updated( const Distributed2DField& data )
{
	// the dataset, to be communicated to the other process by the root one 
	hid_t dataset_id ;
	if ( data.distribution().rank() == 0 ) {
		// Create an HDF5 file to store the varaiable 

		//1) create a file named : file.h5 (HDF5 extention)
		hid_t file_id = H5Fcreate("file.h5", H5F_ACC_TRUNC,H5P_DEFAULT, H5P_DEFAULT);
		//2) set the dimention of the matrix to store inside the 
		hsize_t dims[2] = {data.distribution().extent( DX ),data.noghost_view().extent( DY )};
		// create the dataspace associated to the simulationand create the dataset
		hid_t dataspace_id = H5Screate_simple(2, dims, NULL);
		dataset_id = H5Dcreate(file_id, "S", H5T_STD_I32BE,dataspace_id, H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);
		
		cout << "at t="<<data.time()<<" : [" << endl;
	}
	//Comunicate the dataset_id to the other process using the MPI brodcast 
	MPI_Bcast(&dataset_id, 1,H5T_STD_I32LE, 0, data.distribution().communicator());
	cout<<flush; 
	MPI_Barrier(data.distribution().communicator());

	
	sleep_for(milliseconds(1));
	// preparing the simple for transferting the data to the dataset
	hsize_t dims[1] ;
	hid_t mspace_id = H5Screate_simple(1, dims, NULL);
	// Get the space id for our dataset of the HDF5 file 
	hid_t fspace_id = H5Dget_space(dataset_id);
	// The coords will be determined for every element of the simulation data 
	hsize_t start[2];
	// We write a single element at the time 
	hsize_t count[2] = {1, 1};
	// the array containing the value to write in the dataset
	double value[1];
	for ( int pyy = data.distribution().extent( DY )-1; pyy >=0 ; --pyy ) {
		for ( int yy = data.noghost_view().extent( DY )-1; yy >=0 ; --yy ) {
			for ( int pxx = 0; pxx < data.distribution().extent( DX ); ++pxx ) {

				start[1] = yy;

				if ( data.distribution().coord( DX ) == pxx && data.distribution().coord( DY ) == pyy ) {
					
					for ( int xx = 0; xx < data.noghost_view().extent( DX ); ++xx ) {
						start[0] = xx;
				
						if ( 0 == data.noghost_view(yy, xx) ) {
							value[0] = 0;
						} else {
							value[0] = data.noghost_view(yy, xx);
						}

						// insert the element inside the 
						H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET, start, NULL, count,NULL);
						H5Dwrite(dataset_id, H5T_NATIVE_INT, mspace_id, fspace_id,H5P_DEFAULT, value);

					}
					if ( pxx == data.distribution().extent( DX ) - 1 ) {
						cout << " ]\n";
					}
				}
				cout<<""<<flush;
				MPI_Barrier(data.distribution().communicator());
				sleep_for(milliseconds(1));
			}
		}
	}
	if ( data.distribution().rank() == 0 ) {
		cout << "]" << endl;
		H5Dclose(dataset_id);
		H5Sclose(dataspace_id);
		H5Fclose(file_id); 
	}
	cout<<flush;
	MPI_Barrier(data.distribution().communicator());
	sleep_for(milliseconds(1));
}
