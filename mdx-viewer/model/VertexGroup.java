package org.aprog.mdxviewer.model;

import java.util.ArrayList;
import java.util.Collection;

import javax.vecmath.Matrix4f;

public final class VertexGroup {
	
	private final Collection<Bone> bones;
	
	private final Matrix4f matrix;

	public VertexGroup(final Collection<Bone> bones) {
		this.bones = new ArrayList<Bone>(bones);
		this.matrix = new Matrix4f();
		
		this.matrix.setIdentity();
	}

	final void updateMatrix() {
		final int n = this.bones.size();
		if (n>0) {
			this.matrix.mul(0);
			for (final Bone bone : this.bones) /*if (bone.getName().startsWith("ForeArm"))*/ this.matrix.add(bone.getMatrix());
			this.matrix.mul(1.0f/n);
		}
	}

	public final Matrix4f getMatrix() {
		return (Matrix4f)this.matrix.clone();
	}
	
}
