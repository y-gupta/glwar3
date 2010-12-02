package org.aprog.mdxviewer.model;

import java.util.List;

import org.aprog.mdxviewer.util.Debug;


public final class Transformation1 extends Transformation {

	Transformation1(int interpolation, GlobalSequence globalSequence, List<Key> keys) {
		super(interpolation, globalSequence, keys);
		Debug.checkInput(keys.get(0).getParameters().length==1);
		// TODO Auto-generated constructor stub
	}

}
