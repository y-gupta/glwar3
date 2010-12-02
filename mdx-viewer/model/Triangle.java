package org.aprog.mdxviewer.model;

import org.aprog.mdxviewer.util.Debug;

public final class Triangle {
	
	private final Geoset mdx;
	
	private final int[] vIndices;
	
	Triangle(final Geoset mdx, final int[] vIndices) {
		Debug.checkInput(mdx!=null);
		Debug.checkInput(vIndices.length==3);
		
		this.mdx = mdx;
		this.vIndices = new int[3];
		
		this.setVIndex(0,vIndices[0]);
		this.setVIndex(1,vIndices[1]);
		this.setVIndex(2,vIndices[2]);
	}
	
	public final int getVIndex(final int i) {
		return this.vIndices[i];
	}
	
	private final void setVIndex(final int i, final int vIndex) {
		if (0<=vIndex && vIndex<mdx.getNbVertices()) this.getVIndices()[i] = vIndex;
		else throw new IndexOutOfBoundsException();
	}
	
	public final Vertex getVertex(final int i) {
		return this.getMdx().getVertex(this.getVIndex(i));
	}

	public final Geoset getMdx() {
		return mdx;
	}

	private final int[] getVIndices() {
		return vIndices;
	}

}
