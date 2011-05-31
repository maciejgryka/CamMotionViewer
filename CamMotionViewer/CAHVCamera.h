#ifndef CAHVCAMERA
#define CAHVCAMERA

#define PI 3.14159f

class CAHVCamera {

public:
	CAHVCamera(){};

	/// 6-array with position in [0-2] and rotation in [3-5]
	CAHVCamera(float* vals){
		this->position[0] = vals[0];
		this->position[1] = vals[1];
		this->position[2] = vals[2];
		
		//this->rotation[0] = vals[3];
		//this->rotation[1] = vals[4];
		//this->rotation[2] = vals[5];

		computeSphericals();
	};

	/// same as above, except with custom origin
	CAHVCamera(float* vals, float* center) {
		this->position[0] = vals[0] - center[0];
		this->position[1] = vals[1] - center[1];
		this->position[2] = vals[2] - center[2];
		
		//this->rotation[0] = vals[3];
		//this->rotation[1] = vals[4];
		//this->rotation[2] = vals[5];
		
		computeSphericals();
	};

	/// radius, elevation, azimuth
	CAHVCamera(float r, float theta, float fi) {
		this->r = r;
		this->theta = theta;
		this->fi = fi;

		computeCartesians();
	};

	~CAHVCamera(){};

	// change the world origin
	void center(float center[3]) {
		position[0] -= center[0];
		position[1] += center[1];
		position[2] += center[2];

		computeSphericals();
	};

	// we fix the axes notation to OpenGL style - i.e. default position is camera at the origin looking down the negative Z-axis
	// with X-axis sticking out to the right and Y-axis to the top - the equations below taken from 
	// https://secure.wikimedia.org/wikipedia/en/wiki/Spherical_coordinate_system map the spherical coordinates as follows
	// Wikipedia notation			OpenGL notation
	//			x						z
	//			y						x
	//			z						y
	void computeSphericals() {
		// calculate corresponding spherical coordinates with the center at the origin
		r = sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]);
		theta = asin(-position[1]/r);		// TODO: should this minus be here? for now seems to work...
		fi = atan(position[0]/position[2]);
		// add offset based on which X-Z quadrant we're in
		//        |
		//   +PI  |  +PI
		//        |
		// --------------->
		//        |       x
		//	+2PI  |
		//        v z
		if (position[2] < 0) {
			fi += PI;
		} else if (position[0] < 0) {
			fi += 2*PI;
		}
		fi = 2*PI - fi;
	};

	void computeCartesians() {
		position[0] = r * cos(theta) * cos(fi);
		position[1] = r * sin(theta);
		position[2] = r * cos(theta) * sin(fi);
	};

	float position[3];		// x, y, z as used in OpenGL
	float rotation[3];
	float r, theta, fi;

};

#endif