#include "Player.hpp"
#include "World.hpp"

Player::Player(World &world) :
	Entity(world), selectedID(1), onFloor(false), isJumping(false),
	frustumPlanes(6,std::vector<vec3f> //6 planes
				  (4,vec3f(0,0,0)))	{//4 points per plane
	pos = vec3f(256,0,0);
	hitbox = Hitbox(vec3f(256,0,0), vec3f(0.2,0.9,0.2));
	camPos = pos + vec3f(0,0.6,0);
	acc = vec3f(0,-40,0);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	//move the player's position
	//vel.x += acc.x*deltaTime; Player only accelerates vertically
	vel.y += acc.y*deltaTime;
	//vel.z += acc.z*deltaTime; Player only accelerates vertically
	movePos(deltaTime);
	hitbox.pos = pos;
	camPos = pos + vec3f(0,0.6,0);
	makeFrustum();
}

void Player::draw() const {
	//draw crosshair. this shouldn't be here, it is not a "part" of the 3D enviorment. This
	//should be a sfml draw in SceneMain::draw();
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	glVertex3f(-0.0002,      0,-0.01);
	glVertex3f( 0.0002,      0,-0.01);
	glVertex3f(      0,-0.0002,-0.01);
	glVertex3f(      0, 0.0002,-0.01);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Player::movePos(float deltaTime) {

	vec3f disp = vel*deltaTime; //deltax = vt bitch

	if (!hitbox.collidesWithWorld(parentWorld,vec3f(disp.x,0,0)))
		pos.x += disp.x;
	if (!hitbox.collidesWithWorld(parentWorld,vec3f(0,disp.y,0)))
		pos.y += disp.y;
	if (!hitbox.collidesWithWorld(parentWorld,vec3f(0,0,disp.z)))
		pos.z += disp.z;

	onFloor = hitbox.collidesWithWorld(parentWorld,vec3f(0,0.1,0));
	if (onFloor)
		vel.y = 0;

	isJumping = (vel.y > 0);
	vel.x = 0; // Player only accelerates vertically, so speed.x doesn't carry
	vel.y = std::fmin(10,vel.y);
	vel.z = 0; // Player only accelerates vertically, so speed.z doesn't carry

}

void Player::drawHitBox() const {
	//for debugging purposes
	//	std::vector<std::vector<bool> > used(12,std::vector<bool>(12,false));
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glPushMatrix();
	//	glColor4f(0.0,0.0,0.0,1);
	//	glBegin(GL_LINES);
	//	for (int i = 0; i < 12; ++i) {//12 is the number of points that define the hitbox.
	//		for (int j = 0; j < 12; ++j) {//12 is the number of points that define the hitbox.
	//			if(!used[i][j] && i != j && ((hitBox[i].x == hitBox[j].x && hitBox[i].y == hitBox[j].y)
	//										 || (hitBox[i].x == hitBox[j].x && hitBox[i].z == hitBox[j].z)
	//										 || (hitBox[i].z == hitBox[j].z && hitBox[i].y == hitBox[j].y))) {
	//				glVertex3f(pos.x + hitBox[i].x,pos.y + hitBox[i].y,pos.z + hitBox[i].z);
	//				glVertex3f(pos.x + hitBox[j].x,pos.y + hitBox[j].y,pos.z + hitBox[j].z);
	//				used[i][j] = true;
	//			}
	//		}
	//	}
	//	glEnd();
	//	glColor4f(1.0,1.0,1.0,1.0);
	//	glPopMatrix();
	//TODO
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
		glVertex3f(frustumPlanes[i][0].x,frustumPlanes[i][0].y,frustumPlanes[i][0].z);
		glVertex3f(frustumPlanes[i][1].x,frustumPlanes[i][1].y,frustumPlanes[i][1].z);
		glVertex3f(frustumPlanes[i][2].x,frustumPlanes[i][2].y,frustumPlanes[i][2].z);
		glVertex3f(frustumPlanes[i][3].x,frustumPlanes[i][3].y,frustumPlanes[i][3].z);
		glVertex3f(frustumPlanes[i][0].x,frustumPlanes[i][0].y,frustumPlanes[i][0].z);
		glEnd();
		glColor4f(1.0,1.0,1.0,1.0);
		glPopMatrix();
	}
}

void Player::makeFrustum() {
	//calculate frustum with dir, pos , znear, zfar, fov, screen ratio
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	vec3f
			dir(m[2],m[6],m[10]),//same as the player's pov
			side(m[0], m[4], m[8]), //x of the camera in world coords
			up(m[1], m[5], m[9]); //up vector of the camera i world coords

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
	nc = pos - dir * ZNEAR;
	fc = pos - dir * ZFAR;

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

bool Player::insideFrustum(vec3f center, float radius) const {
	float distance,D;
	for(int i=0; i < 6; i++) {
		//construct the plane with a normal and a point
		vec3f v = frustumPlanes[i][1]-frustumPlanes[i][0];
		vec3f u = frustumPlanes[i][2]-frustumPlanes[i][0];
		vec3f n = cross(v,u); //normal n = (Ax,By,Cz), point [0] = (p1,p2,p3)
		normalize(n);
		D = -dot(n,frustumPlanes[i][0]); //A*p1*x + B*p2*y + C*p3*z + D = 0 => D = -dot(n,P)
		distance = dot(n,center) + D;//Solve the equation using the player's pos instead
		//of a point in the plane.
		if (distance < -radius)
			return false; //not inside this player's view
	}
	return true;
}
