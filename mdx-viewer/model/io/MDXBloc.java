package org.aprog.mdxviewer.model.io;

import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

public final class MDXBloc extends MDXObject {
	
	private final List<Object> fields;
	
	private final Map<String, Integer> fieldIndices;
	
	MDXBloc(final String tag, final SizeType sizeType) {
		super(tag,sizeType);
		this.fields = new ArrayList<Object>();
		this.fieldIndices = new Hashtable<String, Integer>();
	}
	
	/**
	 * Same as this(null,sizeType)
	 * @param sizeType
	 */
	MDXBloc(final SizeType sizeType) {
		this(null,sizeType);
	}
	
	/**
	 * Same as this(SizeType.NONE)
	 *
	 */
	MDXBloc() {
		this(SizeType.NONE);
	}
	
	final MDXBloc add(final String key, final Object value) {
		if (value instanceof MDXObject) ((MDXObject)value).setParent(this);
		if (this.fields.add(value)) this.fieldIndices.put(key,this.fields.size()-1);
		return this;
	}

	final MDXBloc addInt(final String key) {
		return this.add(key,new int[1]);
	}

	@Override
	final MDXObject doCreateBlueprint(final AccessRestrictor ar) {
		final MDXBloc bluePrint = new MDXBloc(super.getTag(),super.getSizeType());
		bluePrint.fieldIndices.putAll(this.fieldIndices);
		for (final Object value: this.fields) {
			if (value instanceof MDXObject) bluePrint.fields.add(((MDXObject)value).createBlueprint().setParent(bluePrint));
			else if (value instanceof int[]) bluePrint.fields.add(new int[Array.getLength(value)]);
			else if (value instanceof short[]) bluePrint.fields.add(new short[Array.getLength(value)]);
			else if (value instanceof char[]) bluePrint.fields.add(new char[Array.getLength(value)]);
			else if (value instanceof float[]) bluePrint.fields.add(new float[Array.getLength(value)]);
			else if (value instanceof StringBuffer) bluePrint.fields.add(new StringBuffer(((StringBuffer)value).capacity()));
			else if (value instanceof String) bluePrint.fields.add(value);
			else throw new ClassCastException(value.getClass().getCanonicalName());
		}
		return bluePrint;
	}

	@Override
	final void doRead(final AccessRestrictor ar, final MDXDataInputStream in) throws IOException {
		try {
			for (final Object o: this.fields) {
				if (in.getNbBytesToSkip()>0)
					if (o instanceof MDXObject) ((MDXObject)o).read(in);
					else in.getClass().getMethod("read",new Class[] { o.getClass() }).invoke(in,o);
			}
		}
		catch (final NoSuchMethodException nsme) { nsme.printStackTrace(); }
		catch (final InvocationTargetException ite) { ite.printStackTrace(); }
		catch (final IllegalAccessException iae) { iae.printStackTrace(); }
	}

	@Override
	final Object doGet(final AccessRestrictor ar, final Object key) {
		final Object index = this.fieldIndices.get((String)key);
		if (index!=null) return this.fields.get((Integer)index);
		return null;
	}

}
