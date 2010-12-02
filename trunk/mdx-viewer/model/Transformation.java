package org.aprog.mdxviewer.model;

import java.util.ArrayList;
import java.util.List;

import javax.vecmath.Quat4f;

import org.aprog.mdxviewer.util.Debug;


public class Transformation {
	
	public static final String TRANSLATION_TAG = "KGTR";
	public static final String SCALING_TAG     = "KGSC";
	public static final String ROTATION_TAG    = "KGRT";
	public static final String VISIBILITY_TAG  = "KATV";
	
	public static final int TRANSLATION_NB_PARAMETERS = 3;
	public static final int SCALING_NB_PARAMETERS     = 3;
	public static final int ROTATION_NB_PARAMETERS    = 4;
	public static final int VISIBILITY_NB_PARAMETERS  = 1;
	
	public static enum Interpolation { NONE, LINEAR, HERMITE, BEZIER }
	
	private final Interpolation interpolation;
	
	private final GlobalSequence globalSequence;
	
	private final List<Key> keys;
		
	Transformation(final int interpolation, final GlobalSequence globalSequence, final List<Key> keys) {
		this.interpolation = Interpolation.values()[interpolation];
		this.globalSequence = globalSequence;
		this.keys = new ArrayList<Key>();
		
		for (final Key key : keys) this.keys.add(key.clone());
		
		this.assertState();
	}
	
	private final void assertState() {
		final Key key0 = this.keys.get(0);
		final int nbParameters = key0.getParameters().length;
		final boolean hasTan = key0.getInTan()!=null;
		int previousTime = 0;
		for (final Key key : this.keys) {
			Debug.assertState(key.getParameters().length==nbParameters);
			Debug.assertState((key.getInTan()!=null)==hasTan);
			final int currentTime = key.getTime();
			Debug.assertState(previousTime==0 || currentTime>=previousTime);
			previousTime = currentTime;
		}
	}
		
	public static final String getTransformationElementParametersName(final String tag) {
		if (tag.equals(TRANSLATION_TAG)) return "xyz";
		else if (tag.equals(SCALING_TAG)) return "xyz";
		else if (tag.equals(ROTATION_TAG)) return "abcd";
		else if (tag.equals(VISIBILITY_TAG)) return "state";
		else throw new IllegalArgumentException(tag);
	}

	public static final int getTransformationElementNbParameters(final String tag) {
		if (tag.equals(TRANSLATION_TAG)) return TRANSLATION_NB_PARAMETERS;
		else if (tag.equals(SCALING_TAG)) return SCALING_NB_PARAMETERS;
		else if (tag.equals(ROTATION_TAG)) return ROTATION_NB_PARAMETERS;
		else if (tag.equals(VISIBILITY_TAG)) return VISIBILITY_NB_PARAMETERS;
		else throw new IllegalArgumentException(tag);
	}

	public final GlobalSequence getGlobalSequence() {
		return globalSequence;
	}

	public final Interpolation getInterpolation() {
		return interpolation;
	}

	public static final class Key implements Cloneable {
		
		private final int time;
		
		private final float[] parameters;
		
		private final float[] inTan, outTan;
		
		public Key(final int time, final float[] parameters, final float[] inTan, final float[] outTan) {
			Debug.checkInput(time>=0);
			Debug.checkInput(inTan==null || parameters.length==inTan.length);
			Debug.checkInput((inTan==null && outTan==null) || inTan.length==outTan.length);
			this.time = time;
			this.parameters = parameters.clone();
			if (inTan!=null && outTan!=null) {
				this.inTan = inTan.clone();
				this.outTan = outTan.clone();
			}
			else this.outTan = this.inTan = null;
		}
		
		Key(final int time, final float[] parameters) {
			this(time,parameters,null,null);
		}
		
		@Override
		protected final Key clone() {
			return new Key(this.time,this.parameters,this.inTan,this.outTan);
		}

		final float[] getInTan() {
			return (this.inTan==null ? null : this.inTan.clone());
		}

		final float[] getOutTan() {
			return (this.outTan==null ? null : this.outTan.clone());
		}

		final float[] getParameters() {
			return this.parameters.clone();
		}

		final int getTime() {
			return time;
		}
		
		@Override
		public final String toString() {
			final StringBuffer sb = new StringBuffer();
			
			sb.append(this.time);
			
			sb.append(" { ");
			for (final float f : this.parameters) sb.append(f);
			sb.append(" }");
			
			if (this.inTan!=null) {
				sb.append(" { ");
				for (final float f : this.inTan) sb.append(f);
				sb.append(" }");
			}
			
			if (this.outTan!=null) {
				sb.append(" { ");
				for (final float f : this.outTan) sb.append(f);
				sb.append(" }");
			}
			
			return sb.toString();
		}
		
	}
	
	/**
	 * 
	 * @param time
	 * @return { start, end } or null if no interval can contain time
	 */
	private final Key[] getInterval(final int time) {
		int i = 0;
		boolean found = false;
		Key start = null;
		Key end = null;
		while (i<this.keys.size()-1 && !found) {
			start = this.keys.get(i);
			end = this.keys.get(i+1);
			++i;
			found = start.getTime()<=time && time<end.getTime();
		}
		if (found) return new Key[] { start, end };
		else return null;
	}
	
	public final float[] getInterpolationResult(int time) {
		if (this.globalSequence!=null) time = this.globalSequence.getTime();
		final Key[] interval = this.getInterval(time);
		if (interval==null) return null;
		final Key start = interval[0];
		final Key end = interval[1];
		switch (this.interpolation) {
		case NONE: return start.getParameters();
		case LINEAR: return interpolateLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time);
		case HERMITE: return interpolateLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time); // TODO
		case BEZIER: return interpolateLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time); // TODO
		}
		Debug.assertState(false);
		return null;
	}
	
	public final float[] getQuaternionInterpolationResult(int time) {
		if (this.globalSequence!=null) time = this.globalSequence.getTime();
		final Key[] interval = this.getInterval(time);
		if (interval==null) return null;
		final Key start = interval[0];
		final Key end = interval[1];
		switch (this.interpolation) {
		case NONE: return start.getParameters();
		case LINEAR: return interpolateQuaternionLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time);
		case HERMITE: return interpolateQuaternionLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time); // TODO
		case BEZIER: return interpolateQuaternionLinear(start.getTime(),start.getParameters(),end.getTime(),end.getParameters(),time); // TODO
		}
		Debug.assertState(false);
		return null;
	}
	
	private static final float[] interpolateLinear(final float startTime, final float[] startValue, final float endTime, final float endValue[], final float time) {
		Debug.checkInput(0<=startTime);
		Debug.checkInput(startTime<=endTime);
		Debug.checkInput(startValue.length==endValue.length);
		final float k = (time-startTime)/(endTime-startTime);
		final float[] result = startValue.clone();
		for (int i=0;i<result.length;++i) result[i] += k*(endValue[i]-startValue[i]);
		return result;
	}

	private static final float[] interpolateQuaternionLinear(final float startTime, final float[] startValue, final float endTime, final float endValue[], final float time) {
		Debug.checkInput(0<=startTime);
		Debug.checkInput(startTime<=endTime);
		Debug.checkInput(startValue.length==endValue.length);
		final float k = (time-startTime)/(endTime-startTime);
		final float[] result = startValue.clone();
		final Quat4f q = new Quat4f(startValue);
		q.interpolate(new Quat4f(endValue),k);
		q.get(result);
		return result;
	}

}
