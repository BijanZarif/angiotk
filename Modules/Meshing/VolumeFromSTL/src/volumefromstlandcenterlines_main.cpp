#include <feel/feelcore/environment.hpp>

#include <volumefromstl.hpp>


int main( int argc, char** argv )
{
    using namespace Feel;

    po::options_description myoptions = VolumeMeshing::options("");

    Environment env( _argc=argc, _argv=argv,
                     _desc=myoptions,
		     _about=about(_name="meshing_volumefromstlandcenterlines",
				  _author="Feel++ Consortium",
				  _email="feelpp-devel@feelpp.org"));


    VolumeMeshing myMeshingVolume("");
    myMeshingVolume.run();

    return 0;
}

