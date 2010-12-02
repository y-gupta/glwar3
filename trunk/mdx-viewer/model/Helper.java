package org.aprog.mdxviewer.model;

import java.util.Collection;

import org.aprog.mdxviewer.util.Debug;
import org.aprog.mdxviewer.util.Util;


public class Helper extends Transformable {
	
	public Helper(final String name, final int type, final Transformable.PivotPoint pivotPoint,
			final Translation translation, final Scaling scaling, final Rotation rotation, final Visibility visibility,
			final boolean root, final Collection<Helper> children) {
		super(name,type,pivotPoint,translation,scaling,rotation,visibility,root,children);
		
		Debug.assertState(Util.testMask(type,Transformable.HELP) || Util.testMask(type,Transformable.BONE));
	}
	
}
