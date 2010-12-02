package org.aprog.mdxviewer.model.io;

import java.io.IOException;
import java.util.Arrays;
import java.util.InputMismatchException;

import org.aprog.mdxviewer.model.Transformation;
import org.aprog.mdxviewer.util.Debug;


public final class MDXModel {
	
	private final MDXBloc version;
	
	private final MDXArray sequences;
	
	private final MDXArray globalSequences;
	
	private final MDXArray materials;
	
	private final MDXArray textures;
	
	private final MDXArray geosets;

	private final MDXArray bones;
	
	private final MDXArray helpers;
	
	private final MDXArray pivotPoints;

	public MDXModel() {
		this.version = new MDXBloc(SizeType.EXCLUDED_NB_BYTES).addInt("version");
		
		this.sequences = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc()
			.add("name",new StringBuffer(MDXDataInputStream.SIZE_OF_NAME))
			.addInt("start")
			.addInt("end")
			.add("moveSpeed",new float[1])
			.addInt("nonLooping")
			.add("rarity",new float[1])
			.addInt("unknown1")
			.add("bounds",new float[7])
		);
		
		this.globalSequences = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc()
			.addInt("duration")
		);
		
		this.materials = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc(SizeType.INCLUDED_NB_BYTES)
			.addInt("priorityPlane")
			.addInt("renderMode")
			.add("layers",new MDXArray("LAYS",null,new MDXBloc(SizeType.INCLUDED_NB_BYTES)
				.addInt("filterMode")
				.addInt("shading")
				.addInt("textureId")
				.addInt("uvAnimId")
				.addInt("uvId")
				.add("alpha",new float[1])
				.add("kmt",new MDXBloc()
					.add("tag",new StringBuffer(MDXDataInputStream.SIZE_OF_TAG))
					.addInt("nunks")
					.addInt("lineType")
					.addInt("globalSeqId")
					// FIXME this code is broken; MDXArray with SizeType.NONE doesn't read the elements
					.add("alpha",new MDXArray(SizeType.NONE,null,new MDXBloc()
						.addInt("frame")
						.add("state",new MDXVariant() {
							@Override
							final MDXObject createMDXObject(final MDXObject parent) {
								final String tag = parent.getParent().get("tag").toString();
								Debug.print(tag);
								if (tag.equals("KMTA")) return new MDXBloc().addInt("");
								else if (tag.equals("KMTF")) return new MDXBloc().add("",new float[1]);
								else throw new InputMismatchException();
							}
						})
						.add("alphas",new MDXVariant() {
							@Override
							final MDXObject createMDXObject(final MDXObject parent) {
								if (parent.getParent().getInt("lineType")>1)
									return new MDXBloc().add("inTan",new float[1]).add("outTan",new float[1]);
								else return new MDXBloc();
							}
						})
					))
				)
			))
		);
		
		this.textures = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc()
			.addInt("replaceableId")
			.add("path",new StringBuffer(MDXDataInputStream.SIZE_OF_PATH))
			.addInt("unknown1")
			.addInt("wrapping")
		);
		
		this.geosets = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc(SizeType.INCLUDED_NB_BYTES)
			.add("vertices",new MDXArray("VRTX",null,new MDXBloc().add("",new float[3])))
			.add("normals",new MDXArray("NRMS",null,new MDXBloc().add("",new float[3])))
			.add("primTypes",new MDXArray("PTYP",null,new MDXBloc().addInt("")))
			.add("primCounts",new MDXArray("PCNT",null,new MDXBloc().addInt("")))
			.add("triangles",new MDXArray("PVTX",null,new MDXBloc().add("",new short[1])))
			.add("groups",new MDXArray("GNDX",null,new MDXBloc().add("",new char[1])))
			.add("groupCounts",new MDXArray("MTGC",null,new MDXBloc().addInt("")))
			.add("matrices",new MDXArray("MATS",null,new MDXBloc().addInt("")))
			.addInt("materialId")
			.addInt("selectionGroup")
			.addInt("selectable")
			.add("bounds",new float[7])
			.add("animBounds",new MDXArray(null,new MDXBloc().add("",new float[7])))
			.add("uvas",new MDXArray("UVAS",null,new MDXArray("UVBS",null,new MDXBloc().add("",new float[2]))))
		);
		
		this.bones = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,createHelperElementBlueprint()
			.addInt("geosetId")
			.addInt("geosetAnimId")
		);
		
		this.helpers = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,createHelperElementBlueprint());
		
		this.pivotPoints = new MDXArray(SizeType.EXCLUDED_NB_BYTES,null,new MDXBloc().add("",new float[3]));
	}
	
	private static final MDXBloc createHelperElementBlueprint() {
		return new MDXBloc()
			.add("obj",new MDXBloc(SizeType.INCLUDED_NB_BYTES)
				.add("name",new StringBuffer(MDXDataInputStream.SIZE_OF_NAME))
				.addInt("objectId")
				.addInt("parent")
				.addInt("type")
				.add("transformation0",createTransformationBlueprint())
				.add("transformation1",createTransformationBlueprint())
				.add("transformation2",createTransformationBlueprint())
				.add("transformation3",createTransformationBlueprint())
		);
	}
	
	private static final MDXObject createTransformationBlueprint() {
		return new MDXBloc()
			.add("tag",new StringBuffer(MDXDataInputStream.SIZE_OF_TAG))
			.add("transformation",new MDXArray(
				new MDXBloc()
					.addInt("lineType")
					.addInt("globalSeqId"),
				new MDXVariant() {
					@Override
					final MDXObject createMDXObject(final MDXObject parent) {
						final String tag = parent.getParent().get("tag").toString();
						return createTransformationElementBlueprint(
							Transformation.getTransformationElementParametersName(tag),
							Transformation.getTransformationElementNbParameters(tag),
							((MDXArray)parent).getFieldObject().getInt("lineType")
						);
					}
				}
			));
	}
	
	private static final MDXObject createTransformationElementBlueprint(final String parametersName, final int nbParameters, final int lineType) {
		return new MDXBloc()
			.addInt("time")
			.add(parametersName,new float[nbParameters])
			.add("tan",new MDXVariant() {
				@Override
				final MDXObject createMDXObject(final MDXObject parent) {
					if (lineType>1) return new MDXBloc().add("inTan",new float[nbParameters]).add("outTan",new float[nbParameters]);
					else return new MDXBloc();
				}
			});
	}
	
	public final MDXModel read(final MDXDataInputStream in) throws IOException {
		in.read("MDLX");
		while (in.available()>0) {
			Debug.print(in.getNbBytesRead());
			final StringBuffer sb = new StringBuffer(MDXDataInputStream.SIZE_OF_TAG);
			in.read(sb);
			final String tag = sb.toString();
			Debug.print(tag+" "+Arrays.toString(tag.getBytes()));
			if (tag.equals("VERS")) {
				this.version.read(in);
				Debug.print(this.version.get("version"));
			}
			else if (tag.equals("SEQS")) {
				this.sequences.read(in);
				Debug.print(this.sequences.getLength());
			}
			else if (tag.equals("GLBS")) {
				this.globalSequences.read(in);
				Debug.print(this.globalSequences.getLength());
			}
			else if (tag.equals("MTLS")) {
				this.materials.read(in);
				Debug.print(this.materials.getLength());
			}
			else if (tag.equals("TEXS")) {
				this.textures.read(in);
				Debug.print(this.textures.getLength());
			}
			else if (tag.equals("GEOS")) {
				this.geosets.read(in);
				Debug.print(this.geosets.getLength());
			}
			else if (tag.equals("BONE")) {
				this.bones.read(in);
				Debug.print(this.bones.getLength());
			}
			else if (tag.equals("HELP")) {
				this.helpers.read(in);
				Debug.print(this.helpers.getLength());
			}
			else if (tag.equals("PIVT")) {
				this.pivotPoints.read(in);
				Debug.print(this.pivotPoints.getLength());
			}
			else new MDXBloc(SizeType.EXCLUDED_NB_BYTES).read(in);
		}
		
		return this;
	}

	public final MDXArray getGeosets() {
		return geosets;
	}

	public final MDXArray getMaterials() {
		return materials;
	}

	public final MDXArray getTextures() {
		return textures;
	}

	public final MDXBloc getVersion() {
		return version;
	}

	public final MDXArray getGlobalSequences() {
		return globalSequences;
	}

	public final MDXArray getSequences() {
		return sequences;
	}

	public final MDXArray getBones() {
		return bones;
	}

	public final MDXArray getHelpers() {
		return helpers;
	}

	public final MDXArray getPivotPoints() {
		return pivotPoints;
	}
}
