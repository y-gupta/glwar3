package org.aprog.mdxviewer.plugins.mdxviewer.view;

import java.util.Collection;
import java.util.Hashtable;
import java.util.Map;
import java.util.Vector;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLEventListener;

import org.aprog.mdxviewer.application.Application;
import org.aprog.mdxviewer.controller.MDXMouseController;
import org.aprog.mdxviewer.model.Camera;
import org.aprog.mdxviewer.model.Geoset;
import org.aprog.mdxviewer.model.MDX;
import org.aprog.mdxviewer.model.Material;
import org.aprog.mdxviewer.model.TextureManager;
import org.aprog.mdxviewer.model.Triangle;
import org.aprog.mdxviewer.model.Vertex;
import org.aprog.mdxviewer.model.Material.Layer;
import org.aprog.mdxviewer.util.Debug;


import com.sun.opengl.util.Animator;
import com.sun.opengl.util.texture.Texture;

public final class MDXView extends GLCanvas implements GLEventListener {
	
	// The following map contains references (origin,xAxis,yAxis,zAxis) as values
	// For debugging purposes
	private static final Map<Object, float[][]> references = new Hashtable<Object, float[][]>();
	
	private static final void renderReferences(final GL gl) {
		for (final float[][] reference : references.values())
			renderReference(gl,reference[0],reference[1],reference[2],reference[3]);
	}

	private static final long serialVersionUID = 1L;

	private final MDX mdx;

	private float thetaX, thetaZ;

	private float zoom;

	private Selector selector;

	private Collection<MDXChangedListener> mdxChangedListeners;
	
	private long tprec;
	
	private Camera camera;

	private static final float INITIAL_ZOOM = 1f;
	
	private static final float[] ORIGIN = new float[] { 0, 0, 0 };
	
	private static final String DEFAULT_CAMERA_NAME = "";
	private static final float DEFAULT_CAMERA_INITIAL_DISTANCE_FROM_TARGET = 1000f;
	private static final float[] DEFAULT_CAMERA_INITIAL_POSITION = new float[] { 0, 0, DEFAULT_CAMERA_INITIAL_DISTANCE_FROM_TARGET };
	private static final float DEFAULT_CAMERA_NEAR_Z_CLIPPING = 5f;
	private static final float DEFAULT_CAMERA_FAR_Z_CLIPPING = 4000f;

	private final Animator animator;
	
	public MDXView(final MDX mdx) {
		this.selector = new Selector((MainFrame) Application.getInstance().getPlugins().get(MainFrame.class.getCanonicalName()));
		this.zoom = INITIAL_ZOOM;
		this.mdxChangedListeners = new Vector<MDXChangedListener>();
		this.thetaX = -90.0f;
		this.thetaZ = -90.0f;
		this.tprec = System.currentTimeMillis();
		this.camera = new Camera(DEFAULT_CAMERA_NAME,DEFAULT_CAMERA_INITIAL_POSITION,ORIGIN,DEFAULT_CAMERA_NEAR_Z_CLIPPING,DEFAULT_CAMERA_FAR_Z_CLIPPING,null,null,null);
		this.mdx = mdx;
		this.mdx.setCamera(this.camera);
		this.add((MDXChangedListener) selector);
		this.notifyMDXChangedListeners();
		Debug.print();
		this.addGLEventListener(this);
		final MDXMouseController mouseController = new MDXMouseController();
		this.addMouseListener(mouseController);
		this.addMouseMotionListener(mouseController);
		this.addMouseWheelListener(mouseController);
		
		this.selector.setVisible(true);
		this.animator = new Animator(this);
		this.animator.start();
		Debug.print();
	}
	
	@Override
	protected void finalize() throws Throwable {
		this.animator.stop();
		super.finalize();
	}

	private final void applyCameraAdjustment(final GL gl) {
		this.camera.rotateAroundTarget(-this.thetaZ,-this.thetaX);
		
		gl.glTranslatef(0,0,-this.camera.getDistanceFromTarget());
		
		gl.glRotatef(-this.camera.getVerticalAngle(),1,0,0);
		gl.glRotatef(-this.camera.getHorizontalAngle(),0,0,1);
		
		final float[] cameraTarget = this.camera.getTarget();
		gl.glTranslatef(-cameraTarget[0],-cameraTarget[1],-cameraTarget[2]);
	}
	
	public final void display(final GLAutoDrawable drawable) {
		final GL gl = drawable.getGL();

		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
		
		this.applyCameraAdjustment(gl);
		
		long t = System.currentTimeMillis();
		long dt = t - tprec;
		tprec = t;
		mdx.update(dt);
		
		renderReference(gl,ORIGIN,X_AXIS.clone(),Y_AXIS.clone(),Z_AXIS.clone());
		renderReferences(gl);

		gl.glColor3f(1.0f, 1.0f, 1.0f);
		
		int[] temp = selector.getGeoOn();
		if (temp!=null) {
			for (int i = 0; i < temp.length; ++i) this.renderNonBlending(mdx.getGeoset(temp[i]), gl);
			for (int i = 0; i < temp.length; ++i) this.renderBlending(mdx.getGeoset(temp[i]), gl);
		}

	}
	
	private static final float AXIS_SCALE = 50f;
	
	private static final float[] X_AXIS = new float[] { 1, 0, 0 };
	private static final float[] Y_AXIS = new float[] { 0, 1, 0 };
	private static final float[] Z_AXIS = new float[] { 0, 0, 1 };
	
	private static final int X = 0;
	//private static final int Y = 1;
	private static final int Z = 2;
	
	private static final void renderReference(final GL gl, final float [] origin, final float[] xAxis, final float[] yAxis, final float[] zAxis) {
		for (int i=X;i<=Z;++i) {
			xAxis[i] = origin[i] + xAxis[i]*AXIS_SCALE;
			yAxis[i] = origin[i] + yAxis[i]*AXIS_SCALE;
			zAxis[i] = origin[i] + zAxis[i]*AXIS_SCALE;
		}
		
		gl.glBegin(GL.GL_LINES);
		{
			// X axis (red)
			gl.glColor3f(1,0,0);
			gl.glVertex3fv(origin,0);
			gl.glVertex3fv(xAxis,0);
			// Y axis (green)
			gl.glColor3f(0,1,0);
			gl.glVertex3fv(origin,0);
			gl.glVertex3fv(yAxis,0);
			// Z axis (blue)
			gl.glColor3f(0,0,1);
			gl.glVertex3fv(origin,0);
			gl.glVertex3fv(zAxis,0);
		}
		gl.glEnd();
	}
	
	private static final void renderTriangles(final GL gl, final Geoset geoset) {
		gl.glBegin(GL.GL_TRIANGLES);
		for (int i = 0; i < geoset.getNbTriangles(); ++i) {
			final Triangle t = geoset.getTriangle(i);

			float[] v = t.getVertex(0).getData();
			gl.glTexCoord2fv(v, Vertex.UV_OFFSET);
			gl.glVertex3fv(v, Vertex.POSITION_OFFSET);

			v = t.getVertex(1).getData();
			gl.glTexCoord2fv(v, Vertex.UV_OFFSET);
			gl.glVertex3fv(v, Vertex.POSITION_OFFSET);

			v = t.getVertex(2).getData();
			gl.glTexCoord2fv(v, Vertex.UV_OFFSET);
			gl.glVertex3fv(v, Vertex.POSITION_OFFSET);
		}
		gl.glEnd();
	}
	
	private void renderNonBlending(Geoset geoset, GL gl) {
		if (geoset != null) {
			final Material material = geoset.getMaterial();
			for (final Layer layer : material.getLayers()) {
				if (layer.getFilterMode() < 2) {
					layer.enable(gl);
					final Texture texture = TextureManager.getTexture(layer.getTextureInfo());
					if (texture != null) {
						texture.enable();
						texture.bind();
					}
					
					renderTriangles(gl, geoset);
					
					if (texture != null) texture.disable();
					layer.disable(gl);
				}
			}
		}
	}
	
	private void renderBlending(Geoset geoset, GL gl) {
		if (geoset != null) {
			final Material material = geoset.getMaterial();
			for (final Layer layer : material.getLayers()) {
				if (layer.getFilterMode() >= 2) {
					layer.enable(gl);
					final Texture texture = TextureManager.getTexture(layer.getTextureInfo());
					if (texture != null) {
						texture.enable();
						texture.bind();
					}
					
					renderTriangles(gl, geoset);
					
					if (texture != null) texture.disable();
					layer.disable(gl);
				}
			}
		}
	}

	public void displayChanged(GLAutoDrawable drawable, boolean modeChanged,
			boolean deviceChanged) {
		// TODO Auto-generated method stub
		Debug.print();
	}
	
	public final void init(final GLAutoDrawable drawable) {
		Debug.print();
		final GL gl = drawable.getGL();
		gl.glEnable(GL.GL_DEPTH_TEST);
		
		TextureManager.clear();
	}

	public final void reshape(final GLAutoDrawable drawable, final int x,
			final int y, final int width, final int height) {
		Debug.print();
		final GL gl = drawable.getGL();

		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();

		gl.glFrustum(-1.0f,1.0f,-1.0f,1.0f,this.camera.getNearZClipping(),this.camera.getFarZClipping());

		gl.glViewport(x, y, width, height);
	}

	public final float getThetaX() {
		return thetaX;
	}

	public final void setThetaX(final float thetaX) {
		this.thetaX = thetaX;
	}

	public final float getThetaZ() {
		return thetaZ;
	}

	public final void setThetaZ(final float thetaZ) {
		this.thetaZ = thetaZ;
	}

	public final float getZoom() {
		return zoom;
	}

	public final void setZoom(final float zoom) {
		if (zoom > 0) this.zoom = zoom;
		this.camera.setDistanceFromTarget(DEFAULT_CAMERA_INITIAL_DISTANCE_FROM_TARGET * zoom);
	}

	public boolean add(MDXChangedListener mdxChangedListener) {
		return mdxChangedListeners.add(mdxChangedListener);
	}

	public boolean remove(MDXChangedListener mdxChangedListener) {
		return mdxChangedListeners.remove(mdxChangedListener);
	}

	public void notifyMDXChangedListeners() {
		for (MDXChangedListener mdxChangedListener : this.mdxChangedListeners) {
			mdxChangedListener.mdxChanged(this.mdx);
		}
	}
}
