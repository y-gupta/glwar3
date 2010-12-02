package org.aprog.mdxviewer.model;

import java.util.InputMismatchException;

import javax.vecmath.Matrix4f;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;

public final class Vertex {
	
	public static final int POSITION_OFFSET = 0;
	public static final int NORMAL_OFFSET = 3;
	public static final int UV_OFFSET = 6;
	
	public static final int X = 0;
	public static final int Y = 1;
	public static final int Z = 2;
	public static final int NX = 3;
	public static final int NY = 4;
	public static final int NZ = 5;
	public static final int U = 6;
	public static final int V = 7;

	private static final int N = 8;
	
	private final float[] data;
	
	private final VertexGroup vertexGroup;
	
	public Vertex(final float[] data, final VertexGroup vertexGroup) {
		if (data.length!=N) throw new InputMismatchException();
		
		this.data = data.clone();
		this.vertexGroup = vertexGroup;
	}
	
	public final float[] getData() {
		final Matrix4f matrix = this.vertexGroup.getMatrix();
		final Point3f p = new Point3f(this.data[X],this.data[Y],this.data[Z]);
		final Vector3f n = new Vector3f(this.data[NX],this.data[NY],this.data[NZ]);
		matrix.transform(p);
		matrix.transform(n);
		return new float[] { p.x, p.y, p.z, n.x, n.y, n.z, this.data[U], this.data[V] };
	}
	
	final VertexGroup getVertexGroup() {
		return vertexGroup;
	}

}
