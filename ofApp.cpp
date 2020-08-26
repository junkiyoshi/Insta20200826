#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	for (int i = 0; i < 20; i++) {

		this->deg_param_list.push_back(ofRandom(360));
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->deg_param_list.size(); i++) {

		int p = (ofGetFrameNum() * 5) % 720;
		if (p == 0) {

			this->deg_param_list[i] = ofRandom(360);
			this->color_list[i] = this->base_color_list[(int)ofRandom(this->base_color_list.size())];
		}

		int d = this->deg_param_list[i] + p;
		int radius = 15 + i * 15;

		if (d < this->deg_param_list[i] + 360) {

			this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 15, this->deg_param_list[i], d, this->color_list[i]);
		}
		else {

			this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 15, d, this->deg_param_list[i] + 720, this->color_list[i]);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->face.draw();
	this->frame.drawWireframe();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, int deg_start, int deg_end, ofColor color) {

	if (deg_start >= deg_end) {	return;	}

	for (int deg = deg_start; deg <= deg_end; deg += 1) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + 1) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + 1) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), 0));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		if (deg == deg_start) { frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 3); }
		if (deg == deg_end) { frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2); }

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(color);
			frame_target.addColor(ofColor(239));
		}
	}	
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}