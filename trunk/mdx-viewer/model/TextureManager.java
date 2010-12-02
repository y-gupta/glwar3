package org.aprog.mdxviewer.model;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;
import java.util.Hashtable;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.media.opengl.GLException;

import org.aprog.mdxviewer.util.Debug;


import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureIO;

public final class TextureManager {
	
	private static final Map<TextureInfo, Texture> textures = new Hashtable<TextureInfo, Texture>();
	
	private static final boolean GENERATE_MIPMAPS = true;
	
	private static final Map<String, BufferedImage> images = new Hashtable<String, BufferedImage>();
	
	public static final Texture getTexture(final TextureInfo textureInfo) {
		Texture texture = textures.get(textureInfo);
		if (texture==null) try {
			final String texturePath = textureInfo.getPath();
			Debug.print(texturePath);
			Debug.print(texturePath.length());
			BufferedImage image = images.get(texturePath);
			if (image==null) {
				final InputStream input = MPQResourceLoader.getAsInputStream(texturePath);
				if (input != null) image = ImageIO.read(input);
			}
			if (image != null) {
				texture = TextureIO.newTexture(image, GENERATE_MIPMAPS);
				textures.put(textureInfo, texture);
			}
		}
		catch (final GLException e) { e.printStackTrace(); }
		catch (final IOException e) { e.printStackTrace(); }
		return texture;
	}
	
	public static final void clear() {
		textures.clear();
	}
	
	public static final class TextureInfo {
		
		private final int replaceableId;
		
		private final String path;
		
		private final int unknown1;
		
		private final int wrapping;

		public TextureInfo(final int replaceableId, final String path, final int unknown1, final int wrapping) {
			this.replaceableId = replaceableId;
			this.path = path;
			this.unknown1 = unknown1;
			this.wrapping = wrapping;
		}

		public final String getPath() {
			return path;
		}

		public final int getReplaceableId() {
			return replaceableId;
		}

		public final int getUnknown1() {
			return unknown1;
		}

		public final int getWrapping() {
			return wrapping;
		}
		
	}
}

