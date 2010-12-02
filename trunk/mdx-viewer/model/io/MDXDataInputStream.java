package org.aprog.mdxviewer.model.io;

import static java.lang.Math.max;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.InputMismatchException;

import org.aprog.mdxviewer.util.Util;


public final class MDXDataInputStream extends DataInputStream {
	
	static final int SIZE_OF_STRING = 0x150;
	static final int SIZE_OF_NAME = 0x50;
	static final int SIZE_OF_PATH = 0x100;
	static final int SIZE_OF_TAG = 4;
	static final int SIZE_OF_BYTE = 1;
	static final int SIZE_OF_SHORT = 2;
	static final int SIZE_OF_INT = 4;
	static final int SIZE_OF_FLOAT = 4;
	
	private long nbBytesToSkip;
	
	private long nbBytesRead;

	public MDXDataInputStream(final InputStream in) {
		super(in);
		this.nbBytesRead = 0;
	}
	
	public final long skip() throws IOException {
		final long n = this.skip(this.getNbBytesToSkip());
		this.decrNbBytesToSkip(n);
		this.incNbBytesRead(n);
		if (this.getNbBytesToSkip()!=0) throw new IOException();
		return n;
	}
	
	private final int readLittleEndianInteger(final int size) throws IOException {
		int result = 0;
		for (int i=0;i<size;++i) result += this.read() << (8*i);
		this.decrNbBytesToSkip(size);
		this.incNbBytesRead(size);
		return result;
	}
	
	public final int read(final int[] result) throws IOException {
		for (int i=0;i<result.length;++i) result[i] = this.readLittleEndianInteger(SIZE_OF_INT);
		return result.length*SIZE_OF_INT;
	}
	
	public final int read(final short[] result) throws IOException {
		for (int i=0;i<result.length;++i) result[i] = (short)this.readLittleEndianInteger(SIZE_OF_SHORT);
		return result.length*SIZE_OF_SHORT;
	}
	
	public final int read(final char[] result) throws IOException {
		for (int i=0;i<result.length;++i) result[i] = (char)this.readLittleEndianInteger(SIZE_OF_BYTE);
		return result.length*SIZE_OF_BYTE;
	}
	
	public final int read(final float[] result) throws IOException {
		final int[] tmp = new int[1];
		for (int i=0;i<result.length;++i) {
			this.read(tmp);
			result[i] = Float.intBitsToFloat(tmp[0]);
		}
		return result.length*SIZE_OF_FLOAT;
	}
	
	public final int read(final String s) throws IOException {
		final int n = s.length();
		final StringBuffer sb = new StringBuffer(n);
		final int nbBytesRead = this.read(sb,n);
		if (!s.equals(sb.toString())) throw new InputMismatchException((this.getNbBytesRead()-n)+": expected: "+s+"; found: "+sb);
		return nbBytesRead;
	}
	
	public final int read(final StringBuffer sb, final int size) throws IOException {
		for (int i=0;i<size;++i) sb.append((char)super.read());
		Util.removeTrailingZeroes(sb);
		this.decrNbBytesToSkip(size);
		this.incNbBytesRead(size);
		return size;
	}
	
	public final int read(final StringBuffer sb) throws IOException {
		return this.read(sb,sb.capacity());
	}

	public final long getNbBytesToSkip() {
		return nbBytesToSkip;
	}

	public final void setNbBytesToSkip(final long nbBytesToSkip) {
		this.nbBytesToSkip = max(0,nbBytesToSkip);
	}
	
	public final void decrNbBytesToSkip(final long n) {
		this.setNbBytesToSkip(this.getNbBytesToSkip()-n);
	}

	public final long getNbBytesRead() {
		return nbBytesRead;
	}

	private final void incNbBytesRead(final long n) {
		this.nbBytesRead += n;
	}

}
