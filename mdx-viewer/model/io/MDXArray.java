package org.aprog.mdxviewer.model.io;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.List;

import org.aprog.mdxviewer.util.Debug;


public final class MDXArray extends MDXObject {
	
	private final SizeType sizeType;
	
	private final MDXObject elementBluePrint;
	
	private final MDXObject fieldObject;
	
	private final List<MDXObject> elements;
	
	MDXArray(final String tag, final SizeType sizeType, final MDXObject fieldObject, final MDXObject elementBluePrint) {
		super(tag,sizeType);
		this.sizeType = sizeType;
		this.elementBluePrint = elementBluePrint.setParent(this);
		this.fieldObject = fieldObject;
		this.elements = new ArrayList<MDXObject>();
	}
	
	MDXArray(final SizeType sizeType, final MDXObject fieldObject, final MDXObject elementBluePrint) {
		this(null,sizeType,fieldObject,elementBluePrint);
	}
	
	MDXArray(final String tag, final MDXObject fieldObject, final MDXObject elementBluePrint) {
		this(tag,SizeType.NB_ELEMENTS,fieldObject,elementBluePrint);
	}
	
	MDXArray(final MDXObject fieldObject, final MDXObject elementBluePrint) {
		this(null,SizeType.NB_ELEMENTS,fieldObject,elementBluePrint);
	}
	
	/**
	 * Return the number of elements
	 * @return the number of elements
	 */
	public final int getLength() {
		return this.elements.size();
	}

	private final void readComponent(final MDXDataInputStream in) throws IOException {
		this.elements.add(this.elementBluePrint.createBlueprint().setParent(this).read(in));
	}
	
	@Override
	final void doRead(final AccessRestrictor ar, final MDXDataInputStream in) throws IOException {
		if (this.getTag()!=null && this.getTag().equals("LAYS")) Debug.print(new Formatter().format("%x",in.getNbBytesRead()-8));
		
		final MDXObject fieldObject = this.getFieldObject();
		if (fieldObject!=null) fieldObject.read(in);
		
		long remaining = super.getSize();
		while (remaining>0) {
			final long previousNbBytesRead = in.getNbBytesRead();
			this.readComponent(in);
			final long nbBytesRead = in.getNbBytesRead()-previousNbBytesRead;
			if (nbBytesRead==0 || in.getNbBytesToSkip()==0) remaining = 0;
			else if (this.sizeType==SizeType.NB_ELEMENTS) --remaining;
			else remaining -= nbBytesRead;
		}
	}
	
	@Override
	final MDXObject doCreateBlueprint(final AccessRestrictor ar) {
		final MDXObject fieldObject = this.getFieldObject();
		return new MDXArray(this.getTag(),this.sizeType,(fieldObject==null ? null : fieldObject.createBlueprint()),this.elementBluePrint.createBlueprint());
	}
	
	@Override
	final Object doGet(final AccessRestrictor ar, final Object key) {
		return this.elements.get((Integer)key);
	}

	public final MDXObject getFieldObject() {
		return fieldObject;
	}

}

