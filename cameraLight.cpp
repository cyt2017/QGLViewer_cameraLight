/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.1.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "cameraLight.h"

using namespace std;
using namespace qglviewer;

void Viewer::draw() {
  // Place light at camera position 把光线位置设置为摄像机位置
    //!qglviewer::Camera *camera_;QGLViewer中设置了一个摄像机的私有变量
  const Vec cameraPos = camera()->position();
  //平行光源, GL_POSITION属性的最后一个参数为0
  //聚光灯光源, 需要指定位置、方向、光锥半角
  //!点光源, GL_POSITION属性的最后一个参数为1
  const GLfloat pos[4] = {(float)cameraPos[0], (float)cameraPos[1],
                          (float)cameraPos[2], 1.0f};
  glLightfv(GL_LIGHT1, GL_POSITION, pos);

  // Orientate light along view direction 视方向光
  //!GL_SPOT_DIRECTION 指定光照方向
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, camera()->viewDirection());

  drawSpiral();//绘制螺旋模型
}

// Draws a spiral
void Viewer::drawSpiral() {
  const float nbSteps = 1000.0;

  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i < nbSteps; ++i) {
    const float ratio = i / nbSteps;
    const float angle = 21.0 * ratio;
    const float c = cos(angle);
    const float s = sin(angle);
    const float r1 = 1.0 - 0.8f * ratio;
    const float r2 = 0.8f - 0.8f * ratio;
    const float alt = ratio - 0.5f;
    const float nor = 0.5f;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05f, r2 * s);
  }
  glEnd();
}

void Viewer::init() {
  // Light setup 使能光线
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // Light default parameters
  const GLfloat light_ambient[4] = {1.0, 1.0, 1.0, 1.0};//ambient环境光
  const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};//specular镜面反射
  const GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};//diffuse漫反射

  //GL_SPOT_EXPONENT
  //!属性只有一个值，表示聚光的程度，为零时表示光照范围内向各方向发射的光线强度相同，
  //!为正数时表示光照向中央集中，正对发射方向的位置受到更多光照，其它位置受到较少光照。
  //!数值越大，聚光效果就越明显。
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
  //GL_SPOT_CUTOFF
  //!属性也只有一个值，表示一个角度，它是光源发射光线所覆盖角度的一半，
  //!其取值范围在0到90之间，也可以取180这个特殊值。
  //!取值为180时表示光源发射光线覆盖360度，即不使用聚光灯，向全周围发射。即一个点光源。
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
  //GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION。
  //通过设置这三个常数，就可以控制光线在传播过程中的减弱趋势
  //GL_CONSTANT_ATTENUATION
  //!表示光线按常熟衰减(与距离无关)
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1f);
  //GL_LINEAR_ATTENUATION
  //!表示光线按距离线性衰减
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.3f);
  //GL_QUADRATIC_ATTENUATION
  //!表示光线按距离以二次函数衰减
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3f);
  //GL_AMBIENT
  //!表示各种光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度（颜色）。
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  //GL_SPECULAR
  //!表示光线照射到该材质上，经过镜面反射后形成的光线强度（颜色）。
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  //GL_DIFFUSE
  //!表示光线照射到该材质上，经过漫反射后形成的光线强度（颜色）
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

  // Restore previous viewer state.
  restoreStateFromFile();//!刷新之前的窗口

  // Opens help window
  help();
}

QString Viewer::helpString() const {
  QString text("<h2>C a m e r a L i g h t</h2>");
  text += "See the <b>Mouse</b> tab and the documentation web pages for "
          "details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}
