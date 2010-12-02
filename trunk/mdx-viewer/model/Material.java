package org.aprog.mdxviewer.model;

import javax.media.opengl.GL;

import org.aprog.mdxviewer.util.Util;


public final class Material {
	
	public static final class RenderMode {
		
		public static final int CONSTANT_COLOR         = 0x01;
		public static final int CONSTANT_UNKNOWN_1     = 0x02;
		public static final int CONSTANT_UNKNOWN_2     = 0x04;
		public static final int SORT_PRIMITIVES_NEAR_Z = 0x08;
		public static final int SORT_PRIMITIVES_FAR_Z  = 0x10;
		public static final int FULL_RESOLUTION        = 0x20;
		
	}
	
	private final int priorityPlane;
	
	private final int renderMode;
	
	private final Iterable<Layer> layers;
	
	public Material(final int priorityPlane, final int renderMode, final Iterable<Layer> layers) {
		this.priorityPlane = priorityPlane;
		this.renderMode = renderMode;
		this.layers = layers;
	}
	
	public final Iterable<Layer> getLayers() {
		return layers;
	}

	public final int getPriorityPlane() {
		return priorityPlane;
	}

	public final int getRenderMode() {
		return renderMode;
	}
	
	@Override
	public final String toString() {
		final StringBuffer sb = new StringBuffer("<material priorityPlane=\""+this.priorityPlane+"\" renderMode=\""+this.renderMode+"\">\n");
		for (final Layer layer : this.layers) sb.append(layer);
		sb.append("</material>\n");
		return sb.toString();
	}

	public static final class Layer {
		
		public static final class FilterMode {
			
			public static final int NONE        = 0;
			public static final int TRANSPARENT = 1;
			public static final int BLEND       = 2;
			public static final int ADDITIVE    = 3;
			public static final int ADDALPHA    = 4;
			public static final int MODULATE    = 5;
			public static final int MODULATE2X  = 6;
			
		}
		
		public static final class ShadingFlag {
			
			public static final int UNSHADED               = 0x01;
			public static final int SPHERE_ENVIRONMENT_MAP = 0x02;
			public static final int UNKNOWN_1              = 0x04;
			public static final int UNKNOWN_2              = 0x08;
			public static final int TWO_SIDED              = 0x10;
			public static final int UNFOGGED               = 0x20;
			public static final int NO_DEPTH_TEST          = 0x40;
			public static final int NO_DEPTH_SET           = 0x80;
			
		}
		
		private final int filterMode;
		
		private final int shading;
		
		//private final int textureId;
		private final TextureManager.TextureInfo textureInfo;
		
		private final int uvAnimId;
		
		private final int uvId;
		
		private final float alpha;
		
		public Layer(final int filterMode, final int shading, final TextureManager.TextureInfo textureInfo, final int uvAnimId, final int uvId, final float alpha) {
			this.filterMode = filterMode;
			this.shading = shading;
			this.textureInfo = textureInfo;
			this.uvAnimId = uvAnimId;
			this.uvId = uvId;
			this.alpha = alpha;
		}

		public final float getAlpha() {
			return alpha;
		}

		public final int getFilterMode() {
			return filterMode;
		}

		public final int getShading() {
			return shading;
		}

		public final TextureManager.TextureInfo getTextureInfo() {
			return textureInfo;
		}

		public final int getUvAnimId() {
			return uvAnimId;
		}

		public final int getUvId() {
			return uvId;
		}
		
		@Override
		public final String toString() {
			return "<layer filterMode=\""+this.filterMode+"\" shading=\""+this.shading+"\" textureInfo=\""+this.textureInfo+"\" uvAnimId=\""+this.uvAnimId+"\" uvId=\""+this.uvId+"\" alpha=\""+this.alpha+"\"/>\n";
		}
		
		private static final float ALPHA_THRESHOLD = 0.9f;
		
		public final void enable(final GL gl) {
			switch (this.filterMode) {
			case FilterMode.NONE:
				break;
			case FilterMode.TRANSPARENT:
				gl.glEnable(GL.GL_ALPHA_TEST);
				gl.glAlphaFunc(GL.GL_GEQUAL,ALPHA_THRESHOLD);
				break;
			case FilterMode.BLEND:
				gl.glEnable(GL.GL_BLEND);
				gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
				break;
			case FilterMode.ADDITIVE:
				gl.glEnable(GL.GL_BLEND);
				gl.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_DST_ALPHA);
				break;
			case FilterMode.ADDALPHA:
				// TODO
				break;
			case FilterMode.MODULATE:
				// TODO
				break;
			case FilterMode.MODULATE2X:
				// TODO
				break;
			default:
				Util.assertFalse();
			}
		}
		
		public final void disable(final GL gl) {
			switch (this.filterMode) {
			case FilterMode.NONE:
				break;
			case FilterMode.TRANSPARENT:
				gl.glDisable(GL.GL_ALPHA_TEST);
				break;
			case FilterMode.BLEND:
				gl.glDisable(GL.GL_BLEND);
				break;
			case FilterMode.ADDITIVE:
				gl.glDisable(GL.GL_BLEND);
				break;
			case FilterMode.ADDALPHA:
				// TODO
				break;
			case FilterMode.MODULATE:
				// TODO
				break;
			case FilterMode.MODULATE2X:
				// TODO
				break;
			default:
				Util.assertFalse();
			}
		}
	}

}

