package org.aprog.mdxviewer.model;

public class Sequence {

	private int end;
	
	private int start;
	
	private int nonLooping;
	
	/**
	 * Vitesse de dfilement du sol lors du test de l'animation (pas d'utilit dans le jeu)
	 */
	private float moveSpeed;
	
	/**
	 * Influence la slection alatoire d'une animation d'un certain type lorsque plusieurs sont prsentes (eg : Stand, Stand 2, Stand 3)
	 */
	private float rarity;
	
	private String name;
	
	private float[] bounds;
	
	// ajouter unknown1 ?
	public Sequence(String name){
		this.name = name;
	}
	
	public Sequence(String name, int start, int end, int nonlooping, float movespeed, float rarity, float[] bounds){
		this.name = name;
		this.start = start;
		this.end = end;
		this.nonLooping = nonlooping;
		this.moveSpeed = movespeed;
		this.rarity = rarity;
		this.bounds = bounds;
		//Debug.log(name+" "+start+" "+end+" "+nonLooping+" "+movespeed+" "+rarity);
		//Debug.log(name+" "+start+" "+end);
	}

	public String getName() {
		return name;
	}

	public int getEnd() {
		return end;
	}

	public int getStart() {
		return start;
	}
	
}
