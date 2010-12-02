package org.aprog.mdxviewer.model;

import java.util.List;

import org.aprog.mdxviewer.util.Debug;


public final class Rotation extends Transformation {

	public Rotation(final int interpolation, final GlobalSequence globalSequence, final List<Key> keys) {
		super(interpolation,globalSequence,keys);
		Debug.checkInput(keys.get(0).getParameters().length==ROTATION_NB_PARAMETERS);
	}
	
}
