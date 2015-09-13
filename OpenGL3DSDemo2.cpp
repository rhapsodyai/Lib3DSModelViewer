
#define CAM_FACTOR 5
#include <iostream>
#include <vector>
#include <cmath>
#include <gl/glut.h>
#include <lib3ds/lib3ds.h>
#include "Camera.h"

unsigned int rotateSwitch = 1;
char *modelname = "Diamond8.3ds";


Lib3dsFile *m_model;
Lib3dsMesh *mesh;
Camera cam;

//Data Structure to store mesh normals
struct MeshNormal
{
        float (*vertnormals)[3];
};
std::vector<MeshNormal> meshNormalAry;


struct MeshIndex
{
        std::vector<int> faceId;
};

//Data Structure to store materials
struct Materials
{
        float   ambient[4];
        float   diffuse[4];
        float   specular[4];
        float   shininess;
        std::vector<MeshIndex> meshIdx;

};
std::vector<Materials> materials;


//Default Lighting Values (Modifiable)
Materials MatDefault=
{/*
        {0.2f, 0.2f, 0.2f, 1.0f},
        {0.8f, 0.0f, 0.8f, 0.3f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        0.0f
}
*/
        {0.2f, 0.2f, 0.2f, 1.0f},
        {0.8f, 0.0f, 0.8f, 0.3f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        0.0f
        }
;

int materialId;//For Material Management

float scales = 1.f;

//Draw Mesh Without Materials
void DrawWithoutMaterial()
{
        glBegin(GL_TRIANGLES);
        //Loop for meshes
        for(int mLoop = 0; mLoop < m_model->nmeshes;++mLoop)
        {
                mesh = m_model->meshes[mLoop];//Pointer for the mesh on the mLoopth iteration

                //Loop for faces
                for(int loopX = 0;loopX < mesh->nfaces;++loopX)
                {
                        //1st vertex
                        glNormal3f(
                                meshNormalAry[mLoop].vertnormals[loopX*3][0],
                                meshNormalAry[mLoop].vertnormals[loopX*3][1],
                                meshNormalAry[mLoop].vertnormals[loopX*3][2]);
                        glVertex3f(
                                mesh->vertices[ mesh->faces[loopX].index[0] ][0],
                                mesh->vertices[ mesh->faces[loopX].index[0] ][1],
                                mesh->vertices[ mesh->faces[loopX].index[0] ][2]);

                        //2nd vertex
                        glNormal3f(
                                meshNormalAry[mLoop].vertnormals[loopX*3+1][0],
                                meshNormalAry[mLoop].vertnormals[loopX*3+1][1],
                                meshNormalAry[mLoop].vertnormals[loopX*3+1][2]);
                        glVertex3f(
                                mesh->vertices[ mesh->faces[loopX].index[1] ][0],
                                mesh->vertices[ mesh->faces[loopX].index[1] ][1],
                                mesh->vertices[ mesh->faces[loopX].index[1] ][2]);

                        //３rd vertex
                        glNormal3f(
                                meshNormalAry[mLoop].vertnormals[loopX*3+2][0],
                                meshNormalAry[mLoop].vertnormals[loopX*3+2][1],
                                meshNormalAry[mLoop].vertnormals[loopX*3+2][2]);
                        glVertex3f(
                                mesh->vertices[ mesh->faces[loopX].index[2] ][0],
                                mesh->vertices[ mesh->faces[loopX].index[2] ][1],
                                mesh->vertices[ mesh->faces[loopX].index[2] ][2]);
                }

        }
        glEnd();
}

//Draw Mesh with Material
void DrawWithMaterial()
{
        static std::vector<int>::iterator Iter;
        static std::vector<int>::iterator EndIter;
                
        for(int k = 0; k < m_model->nmaterials;++k)//Material loop
        {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[ k ].ambient); 
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[ k ].diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[ k ].specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials[ k ].shininess);
                
                glBegin(GL_TRIANGLES);
                for(int n = 0 ; n < m_model->nmeshes;++n)//Mesh loop
                {
                        mesh = m_model->meshes[ n ];
                        EndIter = materials[k].meshIdx[n].faceId.end();
                        
                        for(Iter = materials[k].meshIdx[n].faceId.begin(); Iter != EndIter;++Iter)//Index loop
                        {
                                glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3 ][0]  );
                                glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[0] ][0]);

                                glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3+1 ][0]  );
                                glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[1] ][0]);

                                glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3+2 ][0]  );
                                glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[2] ][0]);
                        }
                }
                glEnd();
        }
        
        //Viewer Settings for the material
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatDefault.ambient); //Set the id of each face of the material
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDefault.diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatDefault.specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatDefault.shininess);
        glBegin(GL_TRIANGLES);
        for(int n = 0 ; n < m_model->nmeshes;++n)
        {
                mesh = m_model->meshes[ n ];//for the nth mesh + for the nth material face
                EndIter =MatDefault.meshIdx[n].faceId.end();
                for(Iter = MatDefault.meshIdx[n].faceId.begin();Iter != EndIter;++Iter)
                {
                        glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3 ][0]  );
                        glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[0] ][0]);

                        glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3+1 ][0]  );
                        glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[1] ][0]);

                        glNormal3fv( &meshNormalAry[n].vertnormals[ (*Iter)*3+2 ][0]  );
                        glVertex3fv(&mesh->vertices[ mesh->faces[ (*Iter) ].index[2] ][0]);
                }
        }
        glEnd();
        
}

void display()
{
        static int angle = 0;
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        //gluLookAt(20,30,80,0,10,0,0,1,0); //Standard view
        gluLookAt(cam.eyeX+20,cam.eyeY+30,cam.eyeZ+80,cam.centerX+0,cam.centerY+10,cam.centerZ+0,0,1,0);

        static float Light0Pos[]={-1,1,1,0};
        glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);

        
        glPushMatrix();
        if(rotateSwitch==1)
                           glRotatef(float(angle),0,1,0);
        if(rotateSwitch==2)
                           glRotatef(float(angle),1,0,0);
        if(rotateSwitch==3)
                           glRotatef(float(angle),0,0,1);
        glScalef(scales,scales,scales);
        glRotatef(90,-1,0,0);
        
        if(materialId == -999)//Determine if there is material
        {
                DrawWithoutMaterial();
        }else
        {
                DrawWithMaterial();
        }

        glPopMatrix();
        glColor3f(1,1,1);
        glutSwapBuffers();
        if(++angle >= 360) angle = 0;

}


void reshape(int w, int h)
{
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30,double(w)/h,0.1,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}


void idle()
{
        glutPostRedisplay();
}

void specialkey(int key,int x, int y)
{
     if( key == GLUT_KEY_UP )//Up arrow
     {
         cam.eyeX+= CAM_FACTOR;
         cout << "Camera X：" << cam.eyeX << endl;
     }

     if( key == GLUT_KEY_DOWN )//Down arrow
     {
         cam.eyeX-= CAM_FACTOR;
         cout << "Camera X：" << cam.eyeX << endl;
     }

     if( key == GLUT_KEY_LEFT ) //Left Arrow
     {
         cam.eyeY-= CAM_FACTOR;
         cout << "Camera Y：" << cam.eyeY << endl;
     }

     if( key == GLUT_KEY_RIGHT ) //Right Arrow
     {
         cam.eyeY+= CAM_FACTOR;
         cout << "Camera Y：" << cam.eyeY << endl;
     }
     glutSwapBuffers();
}

void init()
{
        glClearColor(0.8f,0.8f,0.8f,1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
        glShadeModel(GL_SMOOTH); //Gourad Shading
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

void keyboard(unsigned char key, int x, int y)
{
switch(key){
        case 27: //esc
            exit(0);
            break;
        case 32: //space
            cam.printCamera();
            break;
        case 113: //q
            cam.eyeZ+=CAM_FACTOR;
            cout << "Camera Z：" << cam.eyeZ << endl;
            break;
        case 97: //a
            cam.eyeZ-=CAM_FACTOR;
            cout << "Camera Z：" << cam.eyeZ << endl;
            break;
        case 119: //w
            cam.centerX+=CAM_FACTOR;
            cout << "Center X：" << cam.centerX << endl;
            break;
        case 115: //s
            cam.centerX-=CAM_FACTOR;
            cout << "Center X：" << cam.centerX << endl;
            break;
        case 49: //1
            cam.centerY+=CAM_FACTOR;
            cout << "Center Y：" << cam.centerY << endl;
            break;
        case 50: //2
            cam.centerY-=CAM_FACTOR;
            cout << "Center Y：" << cam.centerY << endl;
            break;
        case 51: //3
            cam.centerZ+=CAM_FACTOR;
            cout << "Center Z：" << cam.centerZ << endl;
            break;
        case 52: //4
            cam.centerZ-=CAM_FACTOR;
            cout << "Center Z：" << cam.centerZ << endl;
            break;
        case 48: //0                                  //Reset Camera
            cam.resetCamera();
            break;
        case 112: //p                                 //Scale 2x
            scales *= 2.f;
            break;
        case 108: //l                                 //Scale 1/2
            scales *= 0.5f;
            break;
        case 114: //r                                 //Stop and Start Rotation
            if (rotateSwitch==1)
               rotateSwitch=0;
            else
               rotateSwitch=1;
            break;
        case 102: //f
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //Change to wireframe
            break;
        case 103: //g
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //Change to Texture fill
            break;
        case 116: //t                                 //Rotate Y and Z Axis
            rotateSwitch++;
            break;
     }
     glutSwapBuffers();
}

void load3dsModel()
{
        m_model = lib3ds_file_open(modelname);
        if(m_model==NULL)
        {
                std::cerr << "Can't open file!\n";
                exit(0);
        }

        meshNormalAry.resize(m_model->nmeshes);

        for(int loop = 0; loop < m_model->nmeshes;++loop)
        {
                mesh = m_model->meshes[loop];//pointer for the mloopth mesh
                meshNormalAry[loop].vertnormals = new float[mesh->nfaces*3][3];
                lib3ds_mesh_calculate_vertex_normals(mesh,&meshNormalAry[loop].vertnormals[0]);
        }
        

        //End if there is no material
        if(m_model->nmaterials == 0)
        {
                std::cout << "There are no materials for this mesh!\n";
                materialId = -999; //return -999
                return;
        }


        //Store material data
        Lib3dsMaterial *material;
                        
        //Allocate memory for the material
        materials.resize(m_model->nmaterials);
        for(int loop = 0; loop < m_model->nmaterials;++loop)
        {
                material= m_model->materials[loop];//number of iterations

                
                //ambient
                memcpy(materials[loop].ambient, material->ambient, sizeof(material->ambient) );
                materials[loop].ambient[3] = 1;
                
                //diffuse
                memcpy(materials[loop].diffuse, material->diffuse, sizeof(material->diffuse) );
                materials[loop].diffuse[3] =1 - material->transparency;//Alpha value is for diffuse lighting only
                
                //shinness
                memcpy(&materials[loop].shininess, &material->shininess, sizeof(material->shininess) );
                if ( materials[loop].shininess == 0.01f){
                        materials[loop].shininess = 20.f;
                }else{
                        materials[loop].shininess *= 128;
                }

                //specular
                memcpy(materials[loop].specular,material->specular,sizeof(material->specular) );
                materials[loop].specular[3]=1;
                
                //allocate memory for material data storage
                materials[loop].meshIdx.resize(m_model->nmeshes);
                for(int n = 0; n < m_model->nmeshes;++n)
                {
                        mesh = m_model->meshes[n];//for the nth mesh
                        materials[loop].meshIdx[n].faceId.reserve(mesh->nfaces / m_model->nmaterials);//allocate memory
                }
        }

        //default material memory allocation
        MatDefault.meshIdx.resize(m_model->nmeshes);
        for(int loop1=0;loop1 < m_model->nmeshes;++loop1)
        {
                mesh = m_model->meshes[loop1];
                MatDefault.meshIdx[loop1].faceId.reserve(mesh->nfaces / m_model->nmaterials);
        }


        //store a mesh data id in the mesh data structure
        static int MatId = 0;
        for(int k = 0; k < m_model->nmeshes;++k)
        {
                mesh = m_model->meshes[k];//for the kth mesh
                for(int i = 0; i < mesh->nfaces; ++i)
                {
                        MatId = mesh->faces[ i ].material;
                        if(MatId == -1)
                        {
                                MatDefault.meshIdx[k].faceId.push_back( i );
                        }else
                        {        
                                materials[MatId].meshIdx[k].faceId.push_back( i );
                        }
                }
                std::vector<int>(MatDefault.meshIdx[k].faceId).swap(MatDefault.meshIdx[k].faceId);
        }

        //allocate memory with shrink - to - fit adjustment
        for(int loop = 0; loop < m_model->nmaterials;++loop)
        {
                for(int j = 0; j < m_model->nmeshes; ++j)
                {
                        std::vector<int>(materials[loop].meshIdx[j].faceId).swap(materials[loop].meshIdx[j].faceId);
                }
                
        }
}


int main(int argc, char *argv[])
{
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
        glutInitWindowSize(1024,768);
        glutCreateWindow("Lib3ds Mesh Viewer");

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutSpecialFunc(specialkey);
        glutKeyboardFunc(keyboard);
        glutIdleFunc(idle);

        init();

        load3dsModel();

        glutMainLoop();

        return 0;
}

