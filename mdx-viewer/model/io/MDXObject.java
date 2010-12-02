package org.aprog.mdxviewer.model.io;
 
import static org.aprog.mdxviewer.util.Util.isArray;

import java.io.IOException;
import java.lang.reflect.Array;

enum SizeType { NB_ELEMENTS, INCLUDED_NB_BYTES, EXCLUDED_NB_BYTES, NONE }

public abstract class MDXObject {
	
	private final String tag;
	
	private final SizeType sizeType;
	
	private final int[] size;
	
	private MDXObject parent;
	
	private boolean locked;
	
	/**
	 * 
	 * @param tag
	 * @param sizeType
	 */
	MDXObject(final String tag, final SizeType sizeType) {
		this.tag = tag;
		this.sizeType = sizeType;
		if (sizeType!=SizeType.NONE) this.size = new int[1];
		else this.size = null;
		this.parent = null;
		this.locked = false;
	}
	
	/**
	 * Return a MDXObject that can read the same fields as this<br>
	 * Call doCreateBlueprint(AccessREstrictor)<br>
	 * @return the blueprint
	 */
	final MDXObject createBlueprint() {
		if (this.locked) throw new IllegalStateException();
		return this.doCreateBlueprint(new AccessRestrictor(new AccessGrant()));
	}
	
	/**
	 * Effector to be defined by subclasses
	 * @param ar
	 * @return
	 */
	abstract MDXObject doCreateBlueprint(AccessRestrictor ar);
	
	/**
	 * Translate data from the {@link MDXDataInputStream}<br>
	 * If a this.tag!=null, call in.read(String)<br>
	 * If this.sizeType!=NONE, call in.readInt()<br>
	 * Call doRead(AccessRestrictor)<br>
	 * Handle MDXDataInputStream.nbBytesToSkip<br>
	 * @param in
	 * @throws IOException
	 */
	final MDXObject read(final MDXDataInputStream in) throws IOException {
		if (this.locked) throw new IllegalStateException();
		this.locked = true;
		final String tag = this.getTag();
		final SizeType sizeType = this.getSizeType();
		final long previousNbBytesToSkip;
		
		if (tag!=null) in.read(tag);
		
		if (sizeType!=SizeType.NONE) {
			in.read(this.size);
			previousNbBytesToSkip = in.getNbBytesToSkip();
			if (sizeType==SizeType.INCLUDED_NB_BYTES) this.size[0] -= MDXDataInputStream.SIZE_OF_INT;
			if (sizeType!=SizeType.NB_ELEMENTS) in.setNbBytesToSkip(this.getSize());
		}
		else previousNbBytesToSkip = in.getNbBytesToSkip();
		final long previousNbBytesRead = in.getNbBytesRead();
		
		this.doRead(new AccessRestrictor(new AccessGrant()),in);
		
		if (sizeType==SizeType.EXCLUDED_NB_BYTES || sizeType==SizeType.INCLUDED_NB_BYTES)
			in.setNbBytesToSkip(previousNbBytesToSkip-(in.getNbBytesRead()-previousNbBytesRead)-in.skip());
		
		return this;
	}
	
	/**
	 * Translate data from the {@link MDXDataInputStream}<br>
	 * @param ar
	 * @param in
	 * @throws IOException
	 */
	abstract void doRead(AccessRestrictor ar, MDXDataInputStream in) throws IOException;

	/**
	 * Return the Object corresponding to key<br>
	 * Call doGet(AccessRestrictor,Object)
	 * @param key
	 * @return null if the key is not found<br>
	 * value[0] if {@link Array}.getLength(value)==1<br>
	 * (({@link MDXBloc})value).get("") if it exists<br>
	 * value else<br>
	 */
	public final Object get(Object key) {
		final Object value = this.doGet(new AccessRestrictor(new AccessGrant()),key);
		if (value!=null && isArray(value) && Array.getLength(value)==1) return Array.get(value,0);
		else if (value instanceof MDXBloc) {
			final Object o = ((MDXBloc)value).get("");
			if (o!=null) return o;
		}
		return value;
	}
	
	/**
	 * Effector to be defined by subclasses
	 * @param ar
	 * @param key
	 * @return
	 */
	abstract Object doGet(AccessRestrictor ar, Object key);
	
	/**
	 * Return the integer associated with the key
	 * @param key
	 * @return the integer associated with the key
	 */
	public final int getInt(Object key) {
		return (Integer)this.get(key);
	}
	
	/**
	 * Return the MDXBloc associated with the key 
	 * @param key
	 * @return the MDXBloc associated with the key
	 */
	public final MDXBloc getMDXBloc(Object key) {
		return (MDXBloc)this.get(key);
	}
	
	/**
	 * Return the MDXArray associated with the key
	 * @param key
	 * @return the MDXArray associated with the key
	 */
	public final MDXArray getMDXArray(Object key) {
		return (MDXArray)this.get(key);
	}
	
	/**
	 * Return the tag of this MDXObject (may be null)
	 * @return the tag of this MDXObject (may be null)
	 */
	public final String getTag() {
		return tag;
	}
	
	/**
	 * Return the SizeType of this MDXObject
	 * @return the SizeType of this MDXObject
	 */
	final SizeType getSizeType() {
		return this.sizeType;
	}
	
	/**
	 * Return the size of this MDXObject<br>
	 * Warning : depending on sizeType, the size may represent a number of elements or a number of bytes
	 * @return the size of this MDXObject
	 */
	final int getSize() {
		if (this.size==null) return 0;
		return this.size[0];
	}
	
	private static final class AccessGrant { /*nop*/ }
	
	static final class AccessRestrictor {
		AccessRestrictor(final AccessGrant ag) { /*nop*/ }
	}

	final MDXObject getParent() {
		return parent;
	}

	final MDXObject setParent(final MDXObject parent) {
		if (this.parent!=null || parent==null) throw new IllegalStateException();
		this.parent = parent;
		return this;
	}
	
}
