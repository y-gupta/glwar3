package org.aprog.mdxviewer.model;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public final class Geoset {
	
	private final List<Vertex> vertices;
	
	private final List<Triangle> triangles;
	
	private final Set<VertexGroup> vertexGroups;
	
	private final Material material;
	
	public Geoset(final List<Vertex> vertices, final List<int[]> vIndicess, final Material material) {
		this.vertices = new ArrayList<Vertex>(vertices);
		this.triangles= new ArrayList<Triangle>();
		this.vertexGroups = new HashSet<VertexGroup>();
		this.material = material;
		
		for (final int[] vIndices: vIndicess) this.triangles.add(new Triangle(this,vIndices));
		for (final Vertex v : this.vertices) this.vertexGroups.add(v.getVertexGroup());
	}
	
	/*
	final void reloadTexture() {
		Debug.print("(re)loading texture: "+this.textureFile);
		try { this.texture = TextureIO.newTexture(this.textureFile,true); }
		catch (final IOException ioe) {
			Debug.print("texture (re)loading failed: "+this.textureFile);
		}
	}
	*/
	
	final void updateMatrices() {
		for (final VertexGroup vertexGroup : this.vertexGroups) vertexGroup.updateMatrix();
	}
	
	public final int getNbVertices() {
		return this.getVertices().size();
	}
	
	public final Vertex getVertex(final int index) {
		return this.getVertices().get(index);
	}
	
	public final Triangle getTriangle(final int index) {
		return this.getTriangles().get(index);
	}
	
	public final int getNbTriangles() {
		return this.getTriangles().size();
	}

	private final List<Triangle> getTriangles() {
		return triangles;
	}

	private final List<Vertex> getVertices() {
		return vertices;
	}

	public final Material getMaterial() {
		return material;
	}

}

