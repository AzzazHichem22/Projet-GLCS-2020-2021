#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <hdf5.h>

using std::atol;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;







int main( int argc, char* argv[] )
{
	hid_t file_id = H5Fopen("file.h5", H5F_ACC_RDWR,H5P_DEFAULT);
	hid_t group_id = H5Gcreate(file_id,"B", H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);
	H5Gclose(group_id);
	H5Fclose(file_id);

	/*hid_t file_id = H5Fcreate("file.h5", H5F_ACC_TRUNC,H5P_DEFAULT, H5P_DEFAULT);
	hsize_t dims[2] = {4, 6};
	hid_t dataspace_id = H5Screate_simple(2, dims, NULL);
	hid_t dataset_id = H5Dcreate(file_id, "A", H5T_STD_I32BE,dataspace_id, H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);
	H5Dclose(dataset_id);
	H5Sclose(dataspace_id);
	H5Fclose(file_id);*/

	return 0;
}
