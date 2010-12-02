package org.aprog.mdxviewer.model;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import org.aprog.mdxviewer.jmpq.MPQ;


public final class MPQResourceLoader {
	
	private static final Map<String, MPQ> mpqs;
	
	static {
		mpqs = new HashMap<String, MPQ>();
	}
	
	public static final InputStream getAsInputStream(final String resourceName) {
		final byte[] data = getAsByteArray(resourceName);
		return (data != null ? new ByteArrayInputStream(data) : null);
	}
	
	public static final byte[] getAsByteArray(final String resourceName) {
		for (final MPQ mpq : mpqs.values()) {
			byte[] data;
			try {
				data = mpq.getFileAsByteArray(resourceName);
				if (data != null && data.length > 0) return data;
			} catch (final IOException e) {
				e.printStackTrace();
			}
		}
		return null;
	}
	
	public static final void addMPQ(final String mpqAbsolutePathname) throws Exception {
		mpqs.put(mpqAbsolutePathname, new MPQ(mpqAbsolutePathname));
	}

	public static final void removeMPQ(final String mpqAbsolutePathname) {
		mpqs.remove(mpqAbsolutePathname);
	}

}
