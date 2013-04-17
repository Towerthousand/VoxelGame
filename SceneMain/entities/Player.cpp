#include "Player.hpp"
#include "../world/World.hpp"
#include "../SceneMain.hpp"
#include "../../Game.hpp"

Player::Player(SceneMain* scene, const vec3f& pos, const vec3f &scale) :
	Entity(scene,pos,scale), selectedID(1), onFloor(false), isJumping(false),
	frustumPlanes(6,std::vector<vec3f> //6 planes
				  (4,vec3f(0,0,0)))	{//4 points per plane
	hitbox.type = Hitbox::BOX;
	hitbox.radius = vec3f(0.6*scale.x,1.6*scale.y,0.6*scale.z);
	camPos = pos + vec3f(0,1.5,0);
	acc = vec3f(0,-40,0);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	//move and update camera position
	movePos(deltaTime); //this handles collisions
	makeFrustum();
	updateCamera();

	//feedback to be used by the scene
	onFloor = hitbox.collidesWithWorld(vec3f(0,-0.1,0));
	isJumping = (vel.y > 0);

	//Limit movement
	vel.x = 0; // Player only accelerates vertically, so speed.x doesn't carry
	vel.y = std::fmax(-70,vel.y);
	vel.z = 0; // Player only accelerates vertically, so speed.z doesn't carry
}

void Player::updateCamera() {
	camPos = pos + vec3f(0,1.5,0);
	viewMatrix = mat4f::fromIdentity();
	viewMatrix.rotate(camRot.x, 1, 0, 0);
	viewMatrix.rotate(camRot.y, 0, 1, 0);
	viewMatrix.translate(-camPos.x, -camPos.y, -camPos.z);
}

void Player::draw() const {
	//draw the model when there is one...
}

void Player::drawFrustum() const {
	//for debugging purposes. Assign makeFrustrum() to a key
	//instead of every update and call drawFrustum() every draw
	//to see the culling from outside.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	for (int i = 0; i < 6; ++i) {
		glPushMatrix();
		glColor4f(0.0,0.0,0.0,1);
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < 5; ++j)
		glVertex3f(frustumPlanes[i][j%4].x,frustumPlanes[i][j%4].y,frustumPlanes[i][j%4].z);
		glEnd();
		glColor4f(1.0,1.0,1.0,1.0);
		glPopMatrix();
	}
}

void Player::makeFrustum() {
	//calculate frustum with dir, pos , znear, zfar, fov, screen ratio
	mat4f view = parentScene->getState().view;
	vec3f
			dir(view(0,2), view(1,2), view(2,2)),//same as the player's pov
			side(view(0,0), view(1,0), view(2,0)), //x of the camera in world coords
			up(view(0,1), view(1,1), view(2,1)); //up vector of the camera in world coords

	float tfov = (float)tan(DEG_TO_RAD * FOV * 0.5) ; //half FOV in rads
	float ratio = float(SCRWIDTH)/float(SCRHEIGHT);
	vec3f
			nc,fc,
			ntl,ntr,nbl,nbr,
			ftl,ftr,fbl,fbr;
	float nh,nw,fh,fw;
	nh = ZNEAR * tfov; //near height
	nw = nh * ratio;   //near width
	fh = ZFAR  * tfov; //far height
	fw = fh * ratio;   //far width

	// compute the centers of the near and far planes
	nc = camPos - dir * ZNEAR;
	fc = camPos - dir * ZFAR;

	// compute the 4 corners of the frustum on the near plane
	ntl = nc + up * nh - side * nw;
	ntr = nc + up * nh + side * nw;
	nbl = nc - up * nh - side * nw;
	nbr = nc - up * nh + side * nw;

	// compute the 4 corners of the frustum on the far plane
	ftl = fc + up * fh - side * fw;
	ftr = fc + up * fh + side * fw;
	fbl = fc - up * fh - side * fw;
	fbr = fc - up * fh + side * fw;

	// compute the six planes
	frustumPlanes[TOP][0] = ntl;
	frustumPlanes[TOP][1] = ftl;
	frustumPlanes[TOP][2] = ftr;
	frustumPlanes[TOP][3] = ntr;

	frustumPlanes[BOTTOM][0] = nbl;
	frustumPlanes[BOTTOM][1] = nbr;
	frustumPlanes[BOTTOM][2] = fbr;
	frustumPlanes[BOTTOM][3] = fbl;

	frustumPlanes[LEFT][0] = nbl;
	frustumPlanes[LEFT][1] = fbl;
	frustumPlanes[LEFT][2] = ftl;
	frustumPlanes[LEFT][3] = ntl;

	frustumPlanes[RIGHT][0] = ntr;
	frustumPlanes[RIGHT][1] = ftr;
	frustumPlanes[RIGHT][2] = fbr;
	frustumPlanes[RIGHT][3] = nbr;

	frustumPlanes[NEAR][0] = ntl;
	frustumPlanes[NEAR][1] = ntr;
	frustumPlanes[NEAR][2] = nbr;
	frustumPlanes[NEAR][3] = nbl;

	frustumPlanes[FAR][0] = ftr;
	frustumPlanes[FAR][1] = ftl;
	frustumPlanes[FAR][2] = fbl;
	frustumPlanes[FAR][3] = fbr;
}

bool Player::insideFrustum( const vec3f &center, float radius) const {
	float distance,D;
	for(int i=0; i < 6; i++) {
		//construct the plane with a normal and a point
		vec3f v = frustumPlanes[i][1]-frustumPlanes[i][0];
		vec3f u = frustumPlanes[i][2]-frustumPlanes[i][0];
		vec3f n = v^u; //normal n = (Ax,By,Cz), point [0] = (p1,p2,p3)
		n.normalize();
		D = -n*frustumPlanes[i][0]; //A*p1*x + B*p2*y + C*p3*z + D = 0 => D = -dot(n,P)
		distance = n*center + D;//Solve the equation using the player's pos instead
		//of a point in the plane.
		if (distance < -radius)
			return false; //not inside this player's view
	}
	return true;
}
