package org.aprog.mdxviewer.model;

import java.util.Collection;
import java.util.List;
import java.util.Vector;

public class MDX {
	
	private Vector<Geoset> geosets;
	
	private Vector<Sequence> sequences;
	
	private Collection<Helper> skeletonroots;
	
	private List<GlobalSequence> globalsequences;
	
	private SequenceTimer timer;
	
	private Camera camera;
	
	public MDX (){
		geosets = new Vector<Geoset>();
		sequences = new Vector<Sequence>();
		this.setTimer(0,0);
	}

	public void addGeoset (Geoset geo) {
		this.geosets.addElement(geo);
	}

	public Geoset getGeoset(int number){
		return this.geosets.elementAt(number);
	}
	
	public Sequence getSequence(int number){
		return this.sequences.elementAt(number);
	}

	public int getNumberOfGeosets() {
		return geosets.size();
	}
	
	public int getNumberOfSequences() {
		return sequences.size();
	}

	public void addSequence (Sequence seq){
		this.sequences.addElement(seq);
	}
	
	public void setSkeletonRoots(Collection<Helper> skeletonroot) {
		this.skeletonroots = skeletonroot;
	}

	public void update(long dt){
//		idem pour les global sequences
		for (final GlobalSequence globalseq : globalsequences){
			globalseq.updateTimer((int)dt);
		}
		this.timer.update((int) dt);
		
		//appelle updateMatrices() de chaque élément de skeletonRoots
		for (final Helper root : skeletonroots){
			root.updateMatrices(this.timer.getTime());
		}
		//appelle updateMarices() de chaque Geoset
		for (int i=0; i<geosets.size(); i++){
			getGeoset(i).updateMatrices();
		}
	}

	public void setGlobalsequences(List<GlobalSequence> globalsequence) {
		this.globalsequences = globalsequence;
	}

	public void setTimer(int ti, int tf) {
		this.timer = new SequenceTimer(ti, tf);
	}

	public SequenceTimer getTimer() {
		return timer;
	}

	public void setTimer(SequenceTimer timer) {
		this.timer = timer;
	}

	public final Camera getCamera() {
		return camera;
	}

	public final void setCamera(final Camera camera) {
		this.camera = camera;
		for (final Helper helper : this.skeletonroots) helper.setCamera(camera);
	}

	public final Helper getSkeletonRoot() {
		return skeletonroots.iterator().next();
	}
	
}
