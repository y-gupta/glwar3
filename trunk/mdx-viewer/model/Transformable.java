package org.aprog.mdxviewer.model;

import java.util.Collection;

import javax.vecmath.Matrix3f;
import javax.vecmath.Matrix4f;
import javax.vecmath.Point3f;
import javax.vecmath.Quat4f;
import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import org.aprog.mdxviewer.util.Debug;
import org.aprog.mdxviewer.util.Util;


public abstract class Transformable {
	
	// BEGIN types
	
	public static final int HELP = 0x0000;
	public static final int BONE = 0x0100;
	public static final int LITE = 0x0200;
	public static final int EVTS = 0x0400;
	public static final int ATCH = 0x0800;
	public static final int CLID = 0x1000;
	
	// END types
	
	// BEGIN properties
	
	public static final int DONT_INHERIT_TRANSLATION = 0x0001;
	public static final int DONT_INHERIT_SCALING     = 0x0002;
	public static final int DONT_INHERIT_ROTATION    = 0x0004;
	public static final int BILLBOARDED              = 0x0008;
	public static final int BILLBOARDED_LOCK_X       = 0x0010;
	public static final int BILLBOARDED_LOCK_Y       = 0x0020;
	public static final int BILLBOARDED_LOCK_Z       = 0x0040;
	public static final int CAMERA_ANCHORED          = 0x0080;
	
	// END properties
	
	public static final int NB_MAX_TRANSFORMATIONS = 4;
	
	public static final int ORIGIN_SIZE = 3;
	
	public static final int ORIGIN_X = 0;
	public static final int ORIGIN_Y = 1;
	public static final int ORIGIN_Z = 2;
	
	private String name;
	
	private final int type;
	
	private final PivotPoint pivotPoint;
	
	private final Translation translation;
	private final Scaling scaling;
	private final Rotation rotation;
	private final Visibility visibility;
	
	private final boolean root;

	private final Collection<Helper> children;
	
	private final Matrix4f matrix;
	
	private Camera camera;
	
	public Transformable(final String name, final int type, final PivotPoint pivotPoint,
			final Translation translation, final Scaling scaling, final Rotation rotation, final Visibility visibility,
			final boolean root, final Collection<Helper> children) {
		this.name = name;
		this.type = type;
		this.pivotPoint = pivotPoint.clone();
		this.translation = translation;
		this.scaling = scaling;
		this.rotation = rotation;
		this.visibility = visibility;
		this.root = root;
		this.children = children;
		this.matrix = new Matrix4f();
		
		this.matrix.setIdentity();
	}
	
	public final void updateMatrices(final int time) {
		Debug.assertState(this.root);
		this.updateMatrices(time,null);
	}
	
	private final void updateMatrices(final int time, final Matrix4f parentMatrix) {
		if (!this.root) this.matrix.setIdentity();
		
		this.applyTranslation(time);
		this.applyRotation(time);
		this.applyScaling(time);
		
		if (!Util.testMask(this.type,BILLBOARDED)) this.applyPivotPointAdjustment();
		else this.applyBillboarding(parentMatrix);

		if (parentMatrix!=null) this.applyParentMatrix(parentMatrix);
		
		for (final Transformable child : this.children) child.updateMatrices(time,(Matrix4f)this.matrix.clone());
	}
	
	private final void applyParentMatrix(final Matrix4f parentMatrix) {
		final Matrix4f mTmp = (Matrix4f)this.matrix.clone();
		this.matrix.mul(parentMatrix,this.matrix);
		
		final Quat4f quat = new Quat4f();
		mTmp.get(quat);
		if (Util.testMask(this.type, DONT_INHERIT_ROTATION)) this.matrix.setRotation(quat);
		final Vector3f vTmp = new Vector3f();
		mTmp.get(vTmp);
		if (Util.testMask(this.type, DONT_INHERIT_TRANSLATION)) this.matrix.setTranslation(vTmp);
		if (Util.testMask(this.type, DONT_INHERIT_SCALING)) this.matrix.setScale(mTmp.getScale());
	}
	
	/**
	 * The billboarded polygons should initially be in the yz plane.
	 * @param parentMatrix
	 */
	private final void applyBillboarding(final Matrix4f parentMatrix) {
		final Matrix4f mTmp = (Matrix4f)this.matrix.clone();

		if (this.camera!=null) {
			final Point3f origin = new Point3f(this.pivotPoint.getData());
			parentMatrix.transform(origin);
			final Vector3f newX = new Vector3f(this.camera.getPosition());
			newX.sub(origin);
			newX.normalize();
			final Vector3f newY = new Vector3f();
			newY.cross(new Vector3f(this.camera.getUp()),newX);
			newY.normalize();
			final Vector3f newZ = new Vector3f();
			newZ.cross(newX, newY);
			final Matrix3f billboardRotation = new Matrix3f(
				newX.x,newY.x,newZ.x,
				newX.y,newY.y,newZ.y,
				newX.z,newY.z,newZ.z
			);
			this.matrix.setRotation(billboardRotation);
			
			this.applyPivotPointAdjustment();
			
			final Vector4f vTmp = new Vector4f();
			mTmp.getColumn(0, vTmp);
			if (Util.testMask(this.type, BILLBOARDED_LOCK_X)) this.matrix.setColumn(0, vTmp);
			mTmp.getColumn(1, vTmp);
			if (Util.testMask(this.type, BILLBOARDED_LOCK_Y)) this.matrix.setColumn(1, vTmp);
			mTmp.getColumn(2, vTmp);
			if (Util.testMask(this.type, BILLBOARDED_LOCK_Z)) this.matrix.setColumn(2, vTmp);
			/*
			final float[] origin3f = new float[3];
			final float[] xAxis = new float[3];
			final float[] yAxis = new float[3];
			final float[] zAxis = new float[3];
			origin.get(origin3f);
			newX.get(xAxis);
			newY.get(yAxis);
			newZ.get(zAxis);
			MDXView.references.put(this,new float[][] { origin3f, xAxis, yAxis, zAxis });
			*/
		}
	}
	
	private final void applyTranslation(final int time) {
		if (this.translation!=null) {
			final float[] translationValue = this.translation.getInterpolationResult(time);
			if (translationValue!=null) this.matrix.setTranslation(new Vector3f(translationValue));
		}
	}
	
	private final void applyRotation(final int time) {
		if (this.rotation!=null) {
			final float[] rotationValue = this.rotation.getQuaternionInterpolationResult(time);
			if (rotationValue!=null) this.matrix.setRotation(new Quat4f(rotationValue));
		}
	}
	
	private final void applyScaling(final int time) {
		if (this.scaling!=null) {
			final float[] scalingValue = this.scaling.getInterpolationResult(time);
			if (scalingValue!=null)
				// XXX I don't know if the scaling is supposed to come first or last...
				// To change the order, exchange the arguments of mul() in the following expression
				this.matrix.mul(this.matrix,new Matrix4f(
						scalingValue[0],0,0,0,
						0,scalingValue[1],0,0,
						0,0,scalingValue[2],0,
						0,0,0,1
				));
		}
	}
	
	private final void applyPivotPointAdjustment() {
		final Vector3f pivot = new Vector3f(this.pivotPoint.getData());
		final Matrix4f m = new Matrix4f();
		m.setIdentity();
		m.setTranslation(pivot);
		this.matrix.mul(m,this.matrix);
		pivot.negate();
		m.setTranslation(pivot);
		this.matrix.mul(this.matrix,m);
	}
	
	public final boolean hasProperty(final int propertyMask) {
		Debug.assertState(0<propertyMask);
		Debug.assertState(propertyMask<0x100);
		return Util.testMask(this.type,propertyMask);
	}

	public final String getName() {
		return name;
	}

	public void setName(final String name) {
		this.name = name;
	}

	public static final class PivotPoint implements Cloneable {
		
		public static final int X = 0;
		public static final int Y = 1;
		public static final int Z = 2;
		
		private final float[] data;
		
		public PivotPoint(final float x, final float y, final float z) {
			this.data = new float[] { x, y, z };
		}
		//rajouter set pivot point et ajouter jardesk clic droit exporter en jar et i rajouter dans particue emitter2 comme name
		@Override
		public final PivotPoint clone() {
			return new PivotPoint(this.data[X],this.data[Y],this.data[Z]);
		}
		
		public final float[] getData() {
			return this.data.clone();
		}
		
		@Override
		public final String toString() {
			final StringBuffer sb = new StringBuffer();
			sb.append("{ ");
			for (final float f : this.data) {
				sb.append(f);
				sb.append(" ");
			}
			sb.append("}");
			return sb.toString();
		}
	}
	
	public final Matrix4f getMatrix() {
		return this.matrix;
	}

	public final Camera getCamera() {
		return camera;
	}

	public final void setCamera(Camera camera) {
		this.camera = camera;
		for (final Transformable child : this.children) child.setCamera(camera);
	}

}
