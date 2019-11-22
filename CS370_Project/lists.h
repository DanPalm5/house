void create_lists()
{
	// cube list
	glNewList(CUBE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	texturecube();
	glPopAttrib();
	glEndList();

	// floor list
	glNewList(FLOOR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[FLOOR_TEXTURE]);
	glScalef(1, wall_height, wall_length);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// ceiling list
	glNewList(CEILING, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[CEILING_TEXTURE]);
	glRotatef(180, 1, 0, 0);
	glScalef(1, wall_height, wall_length);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// wall list
	glNewList(WALL, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[WALL_TEXTURE]);
	glScalef(1, wall_height, wall_length);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// room list
	glNewList(ROOM, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	// 4 walls
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-wall_length * 2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(wall_length * 2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, wall_length * 2);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, -wall_length * 2);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	// floor
	glPushMatrix();
	setColor(GRAY);
	glTranslatef(0, -wall_height, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(floor_scaleX, floor_scaleY, floor_scaleZ);
	glCallList(FLOOR);
	glPopMatrix();

	// ceiling
	glPushMatrix();
	glTranslatef(0, wall_height, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(floor_scaleX, floor_scaleY, floor_scaleZ);
	glCallList(CEILING);
	glPopMatrix();
	glPopAttrib();
	glEndList();// end room list

	// table and chairs
	glNewList(TABLE_CHAIRS, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	glPushMatrix();
	// table
	glTranslatef(CHAIR_TO_CHAIR_DIST / 4, 0, -CHAIR_TO_CHAIR_DIST / 4);
	glScalef(4, 1, 4);
	glCallList(FULL_CHAIR);
	glPopMatrix();
	// 4 chairs
	glPushMatrix();
	glTranslatef(-CHAIR_TO_CHAIR_DIST, 0.0, 0.0);
	glCallList(FULL_CHAIR);
	glTranslatef(CHAIR_TO_CHAIR_DIST * 2, 0.0, 0.0);
	glCallList(FULL_CHAIR);
	glTranslatef(-CHAIR_TO_CHAIR_DIST, 0.0, -CHAIR_TO_CHAIR_DIST);
	glCallList(FULL_CHAIR);
	glTranslatef(0.0, 0.0, CHAIR_TO_CHAIR_DIST * 2);
	glCallList(FULL_CHAIR);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// work of art
	glNewList(ART, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUniform1i(texSampler, 0);
	glUseProgram(textureShaderProg);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, tex_ids[ARTWORK]);
	glTranslatef((wall_length * 2) - 1.0, 0, -5.0f);
	glScalef(0.1f, 4, 4);
	texturecube();
	glPopAttrib();
	glPopMatrix();
	glEndList();

	// fan
	glNewList(FAN, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(VIOLET);
	glTranslatef(0, wall_height / 2, 0);
	glutSolidSphere(fan_radius, fan_slices, fan_stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, wall_height / 2, 0);
	glScalef(1, 0.1, 1);
	glutSolidCube(2.0);
	glTranslatef(-4, 0, 0);
	glutSolidCube(2.0);
	glTranslatef(2, 0, 2);
	glutSolidCube(2.0);
	glTranslatef(0, 0, -4);
	glutSolidCube(2.0);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// fireplace list
	glNewList(FIREPLACE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);

	//actual fire
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[FIRE]);
	glTranslatef((-wall_length * 2) + 0.05, (-wall_height / 2) - 1.25, 0);
	glScalef(1, 2, 2.2);
	texturecube();
	glPopMatrix();

	//mantel
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, tex_ids[MANTLE]);

	setColor(RED);
	glTranslatef((-wall_length * 2) + 0.01, (-wall_height / 2) - 0.5, 0);
	glScalef(1, 3.5, 4);
	glCallList(CUBE);
	glDisable(GL_BLEND);
	glPopMatrix();
	glPopAttrib();
	glEndList();  // end fireplace list

	// door list
	glNewList(DOOR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[DOOR_TEXTURE]);
	glTranslatef(0, (-wall_height / 2) + 1.25, (-wall_length * 2) + 0.5);
	glScalef(5, 6, 0.7);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// window list
	glNewList(WINDOW, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(GLASS);
	glTranslatef(0, -wall_height / 3, (wall_length * 2));
	glScalef(3, 3, 1.1);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair leg
	glNewList(CHAIR_LEG, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_LEG_TEXTURE]);
	setColor(BROWN);
	glScalef(CHAIR_LEG_SCALEX, CHAIR_LEG_SCALEY, CHAIR_LEG_SCALEZ);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair seat
	glNewList(CHAIR_SEAT, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_TEXTURE]);
	setColor(BROWN);
	glTranslatef(0, 0.25, 0);
	glScalef(CHAIR_SEAT_SCALE, CHAIR_SEAT_SCALE_Y, CHAIR_SEAT_SCALE);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// complete chair list
	glNewList(FULL_CHAIR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);

	glPushMatrix();
	glCallList(CHAIR_LEG);
	glTranslatef(-CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glTranslatef(0, 0, CHAIR_WIDTH);
	glCallList(CHAIR_LEG);
	glTranslatef(CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_TEXTURE]);
	glTranslatef(-CHAIR_WIDTH / 2, CHAIR_LEG_TO_SEAT_HEIGHT, CHAIR_WIDTH / 2);
	glCallList(CHAIR_SEAT);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// tree list
	glNewList(TREE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);

	//star on top
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glTranslatef(tree_offset, tree_height + 1, tree_offset*0.9);
	glScalef(0.5, 0.5, 0.5);
	glutSolidIcosahedron();
	glPopMatrix();

	// top
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_TOP_TEXTURE]);
	glTranslatef(tree_offset, (-wall_height / 2) - 1, tree_offset * 0.9);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.75, 0.75, 0.75);
	gluCylinder(tree_top, tree_base, 0.1, tree_height, tree_slices, tree_stacks);
	glPopMatrix();
	//base
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_STUMP_TEXTURE]);
	glTranslatef(tree_offset, -wall_height, tree_offset * 0.9);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.60, 0.60, 1);
	gluCylinder(tree_stump, stump_radius, stump_radius, stump_height, stump_slices, stump_stacks);
	glPopMatrix();


	// cover
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_COVER_TEXTURE]);
	glTranslatef(tree_offset, -wall_height + 0.15f, tree_offset * 0.9);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.75, 0.75, 0.75);
	gluDisk(tree_cover, tree_cover_inner_rad, tree_cover_outer_rad, tree_slices, tree_stacks);
	glDisable(GL_BLEND);
	glPopMatrix();


	// presents

		//1
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_ONE]);
	glTranslatef(tree_offset - 1, -wall_height + 0.15f, tree_offset * 0.6);
	glRotatef(45, 0, 1, 0);
	texturecube();
	glPopMatrix();
	//2
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_TWO]);
	glTranslatef(tree_offset - 2, -wall_height + 0.15f, tree_offset);
	glRotatef(45, 0, 1, 0);

	texturecube();
	glPopMatrix();

	//3
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_THREE]);
	glTranslatef(tree_offset - 4, -wall_height + 0.15f, tree_offset * 0.7);
	texturecube();
	glPopMatrix();

	glPopAttrib();
	glEndList();

	glNewList(MIRROR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	glPushMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();



}