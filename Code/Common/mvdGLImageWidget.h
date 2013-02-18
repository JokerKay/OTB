/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdGLImageWidget_h
#define __mvdGLImageWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"
#include "mvdAbstractModelRenderer.h"
#include "mvdAbstractViewManipulator.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
class ImageModelRenderer;
class ImageViewManipulator;

/** \class GLImageWidget
 *
 */
class Monteverdi2_EXPORT GLImageWidget :
    public QGLWidget
{
  Q_OBJECT;

//
// Public methods.
public:
  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QGLContext* context,
		 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );
  
  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 const QGLFormat& format,
		 QWidget* parent =NULL,
		 const QGLWidget *shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** Constructor */
  // GLImageWidget( QWidget* parent =NULL, Qt::WindowFlags flags=0);

  /** Destructor */
  virtual ~GLImageWidget();

  /** Set Isotropic-zoom level. */
  inline
    void
    SetIsotropicZoom(double value)
  {
    //TODO: Implement setter.
  }

  /** Get Isotropic-zoom level. */
  inline
    double
    GetIsotropicZoom() const
  {
    // TODO: Implement getter.
    return 1.0;
  }

  /** Set image model */
  inline void SetImageModel(AbstractImageModel* model);

//
// Public SLOTS.
public slots:

//
// SIGNALS.
signals:
  void movingMouse();
  void releasingMouse();
  void ModelImageRegionChanged(const ImageRegionType & );

//
// Protected methods.
protected:
  void initializeGL();
  void resizeGL(int widgth, int height);
  void paintGL();
  void mouseMoveEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void wheelEvent( QWheelEvent *event);
  void resizeEvent(QResizeEvent* event);
  void keyPressEvent( QKeyEvent * event );

  /** Update the image to screen transform */
  //void UpdateTransforms(int w, int h);

//
// Protected attributes.
protected:

//
// Private methods.
private:
  /** Construction code (factorizes constructors initializations). */
  void Initialize(AbstractViewManipulator * manipulator,
                  AbstractModelRenderer * renderer);

//
// Private attributes.
private:
  /** Space to screen transform */
  //AffineTransformType::Pointer m_ImageToScreenTransform;
  //AffineTransformType::Pointer m_ScreenToImageTransform;

  /** Event handler pointer */
  AbstractViewManipulator* m_ImageViewManipulator;

  /** Model Renderer pointer */
  AbstractModelRenderer*   m_ImageModelRenderer;

  AbstractImageModel*      m_ImageModel;
};

}// end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdHistogramModel.h"

namespace mvd
{
/*****************************************************************************/
void 
GLImageWidget
::SetImageModel(AbstractImageModel* model)
{
  m_ImageModel = model;

#if 0
  //
  // REFRESH DISPLAY.

  // set the largest possible region of the image
  // TODO:  rename signal name when handling DataSets collections
  // TODO: move signal into mvdApplication and link it to DockWidget
  // and ImageView.
  emit ModelImageRegionChanged(
    model->GetNativeLargestRegion()
  );
#endif
}

}

#endif // __mvdGLImageWidget_h
