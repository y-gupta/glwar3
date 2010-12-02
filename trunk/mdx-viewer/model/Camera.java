package org.aprog.mdxviewer.model;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import org.aprog.mdxviewer.util.Debug;
import org.aprog.mdxviewer.util.Util;


public final class Camera {
	
	private final String name;
	
	private final float[] position, target;
	
	private final float nearZClipping, farZClipping;
	
	private final Transformation1 roll;
	
	private final Translation positionTranslation, targetTranslation;
	
	private int time;
	
	public static final int X = 0;
	public static final int Y = 1;
	public static final int Z = 2;

	public static final int POSITION_NB_ELEMENTS = 3;
	
	/**
	 * 
	 * @param name
	 * @param position must not be null ; length must be POSITION_NB_ELEMENTS
	 * @param target must not be null ; length must be POSITION_NB_ELEMENTS
	 * @param nearZClipping must be < farZClipping
	 * @param farZClipping
	 * @param roll
	 * @param positionTranslation
	 * @param targetTranslation
	 */
	public Camera(final String name, final float[] position, final float[] target, final float nearZClipping, final float farZClipping, final Transformation1 roll, final Translation positionTranslation, final Translation targetTranslation) {
		Debug.checkInput(nearZClipping<farZClipping);
		Debug.checkInput(position.length==POSITION_NB_ELEMENTS);
		Debug.checkInput(target.length==POSITION_NB_ELEMENTS);
		this.name = name;
		this.position = position;
		this.target = target;
		this.nearZClipping = nearZClipping;
		this.farZClipping = farZClipping;
		this.roll = roll;
		this.positionTranslation = positionTranslation;
		this.targetTranslation = targetTranslation;
	}
	
	private static final double clamp(final double value, final double min, final double max) {
		return Math.min(max,Math.max(min,value));
	}
	
	public static final float VERTICAL_ANGLE_MIN = 1f;
	public static final float VERTICAL_ANGLE_MAX = 179f;
	
	/**
	 * Set the spherical coordinates (angles in degrees) around the target<br>
	 * (0,0) is (x,y,z)==(0,0,1)
	 * verticalAngle is clamped to [VERTICAL_ANGLE_MIN,VERTICAL_ANGLE_MAX]
	 * @param horizontalAngle angle (degrees) from the z-axis
	 * @param verticalAngle angle (degrees) from the negative y-axis in the xy plane
	 */
	public final void rotateAroundTarget(final float horizontalAngle, final float verticalAngle) {
		final float hA = Util.degToRad(horizontalAngle);
		final float vA = Util.degToRad(clamp(verticalAngle,VERTICAL_ANGLE_MIN,VERTICAL_ANGLE_MAX));
		final float r = this.getDistanceFromTarget();
		final double rSinVA = r*Math.sin(vA);
		this.position[X] = (float)(this.target[X]+rSinVA*sin(hA));
		this.position[Y] = (float)(this.target[Y]-rSinVA*cos(hA));
		this.position[Z] = (float)(this.target[Z]+r*cos(vA));
	}
	
	public final float[] getUp() {
		// TODO handle roll
		final float hA = Util.degToRad(this.getHorizontalAngle());
		final float vA = Util.degToRad(this.getVerticalAngle());
		final double cosVA = cos(vA);
		return new float[] {
			(float)(-sin(hA)*cosVA),
			(float)(cos(hA)*cosVA),
			(float)(sin(vA))
		};
	}
	
	public final float getDistanceFromTarget() {
		return this.getDirection().length();
	}
	
	public final void setDistanceFromTarget(final float distance) {
		final Vector3f direction = this.getDirection();
		direction.scale(1-distance/direction.length());
		this.position[X] += direction.x;
		this.position[Y] += direction.y;
		this.position[Z] += direction.z;
	}
	
	public final float getHorizontalAngle() {
		final float[] target = this.getTarget();
		final float[] position = this.getPosition();
		final float dx = position[X]-target[X];
		final Vector2f projection = new Vector2f(dx,position[Y]-target[Y]);
		if (projection.length()==0) return 0;
		normalize(projection);
		return Math.signum(dx)*Util.radToDeg(Math.acos(-projection.y));
	}
	
	private static final void normalize(final Vector2f v) {
		v.normalize();

		// Sometimes the |values| are >1 even after normalize()
		v.clampMin(-1);
		v.clampMax(1);
	}
	
	private static final void normalize(final Vector3f v) {
		v.normalize();

		// Sometimes the |values| are >1 even after normalize()
		v.clampMin(-1);
		v.clampMax(1);
	}
	
	/**
	 * 
	 * @return a value between VERTICAL_ANGLE_MIN and VERTICAL_ANGLE_MAX
	 */
	public final float getVerticalAngle() {
		final Vector3f r = this.getDirection();
		normalize(r);
		return Util.radToDeg(Math.acos(-r.z));
	}
	
	private final Vector3f getDirection() {
		final float[] target = this.getTarget();
		final float[] position = this.getPosition();
		return new Vector3f(target[X]-position[X],target[Y]-position[Y],target[Z]-position[Z]);
	}
	
	public final float getFarZClipping() {
		return farZClipping;
	}

	public final float getNearZClipping() {
		return nearZClipping;
	}

	public final float[] getPosition() {
		if (this.positionTranslation==null) return this.position.clone();
		else return this.positionTranslation.getInterpolationResult(this.time);
	}

	public final float getRoll() {
		if (this.roll==null) return 0;
		else return this.roll.getInterpolationResult(this.time)[0];
	}

	public final float[] getTarget() {
		if (this.targetTranslation==null) return this.target.clone();
		else return this.targetTranslation.getInterpolationResult(this.time);
	}

	public final String getName() {
		return name;
	}

	/**
	 * 
	 * @param time must be >=0
	 */
	public final void setTime(int time) {
		Debug.checkInput(0<=time);
		this.time = time;
	}
	
}
