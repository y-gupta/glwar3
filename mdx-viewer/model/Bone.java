package org.aprog.mdxviewer.model;

import java.util.Collection;

import org.aprog.mdxviewer.util.Debug;
import org.aprog.mdxviewer.util.Util;


public final class Bone extends Helper {
	
	private final GeosetAnim geosetAnim;

	public Bone(final String name, final int type, final Transformable.PivotPoint pivotPoint,
			final Translation translation, final Scaling scaling, final Rotation rotation, final Visibility visibility,
			final boolean root, final Collection<Helper> children, final GeosetAnim geosetAnim) {
		super(name,type,pivotPoint,translation,scaling,rotation,visibility,root,children);
		this.geosetAnim = geosetAnim;
		
		Debug.assertState(Util.testMask(type,Transformable.BONE));
	}

	public final GeosetAnim getGeosetAnim() {
		return geosetAnim;
	}
	
}
