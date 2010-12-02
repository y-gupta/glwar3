package org.aprog.mdxviewer.model.io;

import static org.aprog.mdxviewer.util.Util.testMask;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

import org.aprog.mdxviewer.model.Bone;
import org.aprog.mdxviewer.model.Geoset;
import org.aprog.mdxviewer.model.GlobalSequence;
import org.aprog.mdxviewer.model.Helper;
import org.aprog.mdxviewer.model.MDX;
import org.aprog.mdxviewer.model.Material;
import org.aprog.mdxviewer.model.Rotation;
import org.aprog.mdxviewer.model.Scaling;
import org.aprog.mdxviewer.model.Sequence;
import org.aprog.mdxviewer.model.TextureManager;
import org.aprog.mdxviewer.model.Transformable;
import org.aprog.mdxviewer.model.Transformation;
import org.aprog.mdxviewer.model.Translation;
import org.aprog.mdxviewer.model.Vertex;
import org.aprog.mdxviewer.model.VertexGroup;
import org.aprog.mdxviewer.model.Visibility;
import org.aprog.mdxviewer.model.TextureManager.TextureInfo;
import org.aprog.mdxviewer.util.Debug;


public final class MDXIO {
	
	public static final MDX load(final InputStream in) throws IOException {

		final MDXModel model = new MDXModel().read(new MDXDataInputStream(in));
		
		final List<TextureInfo> textureInfos = createTextureInfos(model.getTextures());
		final List<Material> materials = createMaterials(model.getMaterials(), textureInfos);
		Debug.print();
		final List<GlobalSequence> globalSequences = createGlobalSequences(model.getGlobalSequences());
		Debug.print();
		final Map<Integer, Transformable> transformables = new Hashtable<Integer,Transformable>();
		Debug.print();
		final Collection<Helper> skeletonRoots = createSkeletonAndUpdateMap(model,transformables,globalSequences);
		Debug.print();
		final MDX mdx = new MDX();
		mdx.setSkeletonRoots(skeletonRoots);
		mdx.setGlobalsequences(globalSequences);
		final MDXArray sequences = model.getSequences();
		for (int i=0; i< sequences.getLength() ; i++){
			final MDXBloc seq = sequences.getMDXBloc(i);
			mdx.addSequence(new Sequence(seq.get("name").toString(), seq.getInt("start"), seq.getInt("end"), seq.getInt("nonLooping"), (Float)seq.get("moveSpeed"), (Float)seq.get("rarity"), (float[])seq.get("bounds")));
		}
		
		for (int i=0; i< model.getGeosets().getLength(); i++) {
			final MDXBloc geoset = model.getGeosets().getMDXBloc(i);
			mdx.addGeoset(new Geoset(getVertices(geoset,createVertexGroups(geoset,transformables)),getTriangles(geoset),materials.get(geoset.getInt("materialId"))));
		}
		Debug.print();
		return mdx;
	}
	
	private static final List<Material> createMaterials(final MDXArray materialsMDXArray, final List<TextureInfo> textureInfos) {
		final List<Material> materials = new ArrayList<Material>();
		Debug.print("Nb materials: "+materialsMDXArray.getLength());
		for (int i=0;i<materialsMDXArray.getLength();++i) {
			final MDXBloc material = materialsMDXArray.getMDXBloc(i);
			final MDXArray layersMDXArray = material.getMDXArray("layers");
			final Collection<Material.Layer> layers = new ArrayList<Material.Layer>();
			Debug.print("Nb layers: "+layersMDXArray.getLength());
			for (int j=0;j<layersMDXArray.getLength();++j) {
				final MDXBloc layer = layersMDXArray.getMDXBloc(j);
				layers.add(new Material.Layer(layer.getInt("filterMode"), layer.getInt("shading"), textureInfos.get(layer.getInt("textureId")), layer.getInt("uvAnimId"), layer.getInt("uvId"), (Float)layer.get("alpha")));
			}
			materials.add(new Material(material.getInt("priorityPlane"), material.getInt("renderMode"), layers)); // TODO
		}
		Debug.print(materials);
		return materials;
	}
	
	private static final List<TextureInfo> createTextureInfos(final MDXArray texturesMDXArray) {
		final List<TextureInfo> textureInfos = new ArrayList<TextureInfo>();
		for (int i=0;i<texturesMDXArray.getLength();++i) {
			final MDXBloc texture = texturesMDXArray.getMDXBloc(i);
			textureInfos.add(new TextureManager.TextureInfo(texture.getInt("replaceableId"), texture.get("path").toString(), texture.getInt("unknown1"), texture.getInt("wrapping")));
		}
		return textureInfos;
	}
	
	private static final List<GlobalSequence> createGlobalSequences(final MDXArray globalSequencesMDXArray) {
		final List<GlobalSequence> globalSequences = new ArrayList<GlobalSequence>();
		for (int i=0;i<globalSequencesMDXArray.getLength();++i) globalSequences.add(new GlobalSequence(globalSequencesMDXArray.getMDXBloc(i).getInt("duration")));
		return globalSequences;
	}
	
	private static final List<VertexGroup> createVertexGroups(final MDXBloc geosetMDXBloc, final Map<Integer, Transformable> transformables) {
		final List<VertexGroup> vertexGroups = new ArrayList<VertexGroup>();
		final MDXArray groupCounts = geosetMDXBloc.getMDXArray("groupCounts");
		final MDXArray matrices = geosetMDXBloc.getMDXArray("matrices");
		
		for (int i=0,j=0;i<groupCounts.getLength();++i) {
			final Collection<Bone> bones = new ArrayList<Bone>();
			for (int k=0;k<groupCounts.getInt(i);++k) bones.add((Bone)transformables.get(matrices.getInt(j++)));
			vertexGroups.add(new VertexGroup(bones));
		}
		
		return vertexGroups;
	}
	
	private static final Collection<Helper> createSkeletonAndUpdateMap(final MDXModel model, final Map<Integer, Transformable> transformables, final List<GlobalSequence> globalSequences) {
		final Map<Integer, HelperInfo> helperMap = new Hashtable<Integer,HelperInfo>();
		final MDXArray pivotPointsMDXArray = model.getPivotPoints();
		
		retrieveHelpers(pivotPointsMDXArray,model.getBones(),helperMap);
		retrieveHelpers(pivotPointsMDXArray,model.getHelpers(),helperMap);
		
		// creating children lists
		for (final Map.Entry<Integer, HelperInfo> entry : helperMap.entrySet()) {
			Debug.assertState(entry.getKey().equals(entry.getValue().getHelperMDXBloc().getMDXBloc("obj").getInt("objectId")));
			final Integer parentId = entry.getValue().getHelperMDXBloc().getMDXBloc("obj").getInt("parent");
			if (parentId>=0) helperMap.get(parentId).getChildrenObjectIds().add(entry.getKey());
		}
		
		// creating trees
		final Collection<Helper> skeletonRoots = new ArrayList<Helper>();
		for (final HelperInfo helperInfo : helperMap.values())
			if (helperInfo.getHelperMDXBloc().getMDXBloc("obj").getInt("parent")<0)
				skeletonRoots.add(createHelper(helperMap,helperInfo,transformables,NOT_ROOT,globalSequences));
		
		final Helper rootNode = new Helper("ROOT_NODE", Helper.HELP, new Transformable.PivotPoint(0f, 0f, 0f), null, null, null, null, ROOT, skeletonRoots);
		return new ArrayList<Helper>() {
			private static final long serialVersionUID = 1L;
			
			{ this.add(rootNode); }
		};
	}
	
	private static final void retrieveHelpers(final MDXArray pivotPointsMDXArray, final MDXArray helpersMDXArray, final Map<Integer, HelperInfo> helperMap) {
		for (int i=0;i<helpersMDXArray.getLength();++i) {
			final MDXBloc boneMDXBloc = helpersMDXArray.getMDXBloc(i);
			final MDXBloc objMDXBloc = boneMDXBloc.getMDXBloc("obj");
			final Integer objectId = objMDXBloc.getInt("objectId");
			helperMap.put(objectId,new HelperInfo(boneMDXBloc,createPivotPoint(pivotPointsMDXArray,objectId)));
		}
	}
	
	private static final Transformable.PivotPoint createPivotPoint(final MDXArray pivotPointsMDXArray, final int objectId) {
		final float[] data = (float[])pivotPointsMDXArray.get(objectId);
		return new Transformable.PivotPoint(data[0],data[1],data[2]);
	}
	
	private static final boolean ROOT = true;
	private static final boolean NOT_ROOT = false;
	
	private static final Helper createHelper(final Map<Integer, HelperInfo> helperMDXBlocs, final HelperInfo helperInfo, final Map<Integer, Transformable> transformables, final boolean isRoot, final List<GlobalSequence> globalSequences) {
		final MDXBloc objMDXBloc = helperInfo.getHelperMDXBloc().getMDXBloc("obj");
		final Collection<Helper> children = new ArrayList<Helper>();
		
		// Recursivity : creating children
		for (final Integer childObjectId : helperInfo.getChildrenObjectIds())
			children.add(createHelper(helperMDXBlocs,helperMDXBlocs.get(childObjectId),transformables,NOT_ROOT,globalSequences));
		
		final TransformationBuffer tb = new TransformationBuffer(globalSequences);
		
		tb.set(objMDXBloc.getMDXBloc("transformation0"));
		tb.set(objMDXBloc.getMDXBloc("transformation1"));
		tb.set(objMDXBloc.getMDXBloc("transformation2"));
		tb.set(objMDXBloc.getMDXBloc("transformation3"));
		
		final int type = objMDXBloc.getInt("type");
		
		final Helper newHelper;
		if (testMask(type,Transformable.HELP))
			newHelper = new Helper(objMDXBloc.get("name").toString(),objMDXBloc.getInt("type"),helperInfo.getPivotPoint(),
					tb.getTranslation(),tb.getScaling(),tb.getRotation(),tb.getVisibility(),isRoot,children);
		else if (testMask(type,Transformable.BONE))
			newHelper = new Bone(objMDXBloc.get("name").toString(),objMDXBloc.getInt("type"),helperInfo.getPivotPoint(),
					tb.getTranslation(),tb.getScaling(),tb.getRotation(),tb.getVisibility(),isRoot,children,
					null); // TODO handle geosetAnims
		else {
			newHelper = null;
			throw new IllegalArgumentException();
		}
		
		transformables.put(objMDXBloc.getInt("objectId"),newHelper);
		
		return newHelper;
	}
	
	private static final class TransformationBuffer {
		
		private final List<GlobalSequence> globalSequences;
		
		private Translation translation;
		
		private Scaling scaling;
		
		private Rotation rotation;
		
		private Visibility visibility;
		
		TransformationBuffer(final List<GlobalSequence> globalSequences) {
			this.globalSequences = globalSequences;
		}
		
		final void set(final MDXBloc transformationMDXBloc) {
			final Transformation transformation = createTransformation(transformationMDXBloc,this.globalSequences);
			if (transformation instanceof Translation) {
				Debug.assertState(this.translation==null);
				this.translation = (Translation)transformation;
			}
			else if (transformation instanceof Scaling) {
				Debug.assertState(this.scaling==null);
				this.scaling = (Scaling)transformation;
			}
			else if (transformation instanceof Rotation) {
				Debug.assertState(this.rotation==null);
				this.rotation = (Rotation)transformation;
			}
			else if (transformation instanceof Visibility) {
				Debug.assertState(this.visibility==null);
				this.visibility = (Visibility)transformation;
			}
		}

		final Rotation getRotation() {
			return rotation;
		}

		final Scaling getScaling() {
			return scaling;
		}

		final Translation getTranslation() {
			return translation;
		}

		final Visibility getVisibility() {
			return visibility;
		}
		
	}
		
	private static final class HelperInfo {
		
		static final int ROOT_OBJECT_ID = -1;
		
		private final MDXBloc helperMDXBloc;
		
		private final Transformable.PivotPoint pivotPoint;
		
		private final List<Integer> childrenObjectIds;
		
		HelperInfo(final MDXBloc helperMDXBloc, final Transformable.PivotPoint pivotPoint) {
			this.helperMDXBloc = helperMDXBloc;
			this.pivotPoint = pivotPoint;
			this.childrenObjectIds = new ArrayList<Integer>();
		}

		final List<Integer> getChildrenObjectIds() {
			return childrenObjectIds;
		}

		final MDXBloc getHelperMDXBloc() {
			return helperMDXBloc;
		}

		final Transformable.PivotPoint getPivotPoint() {
			return pivotPoint;
		}
		
	}
	
	private static final List<Vertex> getVertices(final MDXBloc geoset, final List<VertexGroup> vertexGroups) {
		final List<Vertex> result = new ArrayList<Vertex>();
		
		final MDXArray vertices = geoset.getMDXArray("vertices");
		final MDXArray uvbs = geoset.getMDXArray("uvas").getMDXArray(0);
		
		for (int i=0;i<vertices.getLength();++i) {
			final float[] vrtx = (float[])vertices.get(i);
			final float[] uv = (float[])uvbs.get(i);
			result.add(new Vertex(new float[] { vrtx[0],vrtx[1],vrtx[2], 0,0,0, uv[0],uv[1] },vertexGroups.get((Character)geoset.getMDXArray("groups").get(i))));
		}
		
		return result;
	}
	
	private static final List<int[]> getTriangles(final MDXBloc geoset) {
		final List<int[]> result = new ArrayList<int[]>();
		
		final MDXArray triangles = geoset.getMDXArray("triangles");
		
		for (int i=0;i<triangles.getLength();i+=3)
			result.add(new int[] { (Short)triangles.get(i),(Short)triangles.get(i+1),(Short)triangles.get(i+2) });
		
		return result;
	}
	
	public static final Transformation createTransformation(final MDXBloc transformationMDXBloc, final List<GlobalSequence> globalSequences) {
		final String tag = transformationMDXBloc.get("tag").toString();
		final MDXArray transformationMDXArray = transformationMDXBloc.getMDXArray("transformation");
		final MDXObject fieldObject = transformationMDXArray.getFieldObject();
		
		final int interpolation = fieldObject.getInt("lineType");
		final int globalSeqId = fieldObject.getInt("globalSeqId");
		final GlobalSequence globalSequence = (globalSequences.size()>0 && globalSeqId>=0 ? globalSequences.get(globalSeqId) : null);
		
		final List<Transformation.Key> elements = new ArrayList<Transformation.Key>();
		for (int i=0;i<transformationMDXArray.getLength();++i) {
			final MDXObject transformationElementMDXObject = (MDXObject)transformationMDXArray.get(i);
			final MDXObject tanMDXBloc = (MDXObject)transformationElementMDXObject.get("tan");
			elements.add(new Transformation.Key(
				transformationElementMDXObject.getInt("time"),
				(float[])transformationElementMDXObject.get(Transformation.getTransformationElementParametersName(tag)),
				(float[])tanMDXBloc.get("inTan"),(float[])tanMDXBloc.get("outTan")
			));
		}
		
		if (tag.equals(Transformation.TRANSLATION_TAG)) return new Translation(interpolation,globalSequence,elements);
		else if (tag.equals(Transformation.SCALING_TAG)) return new Scaling(interpolation,globalSequence,elements);
		else if (tag.equals(Transformation.ROTATION_TAG)) return new Rotation(interpolation,globalSequence,elements);
		else if (tag.equals(Transformation.VISIBILITY_TAG)) return new Visibility(interpolation,globalSequence,elements);
		else return null;
	}

}
