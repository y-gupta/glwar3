package org.aprog.mdxviewer.model;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import javax.swing.tree.DefaultMutableTreeNode;

public class MappedMutableTreeNode extends DefaultMutableTreeNode {

	private static final long serialVersionUID = 1L;

	private final Map<String, MappedMutableTreeNode> childrenmap;

	private final String id;

	public MappedMutableTreeNode(final String id) {
		super(id);
		this.id = id;
		this.childrenmap = new HashMap<String, MappedMutableTreeNode>();
	}

	public MappedMutableTreeNode put(final String key, final MappedMutableTreeNode value) {
		this.add(value);
		return childrenmap.put(key, value);
	}

	public MappedMutableTreeNode get(final Object key) {
		return childrenmap.get(key);
	}

	public void addNewTreeNode(Scanner scanner) {
		if (scanner.hasNext()) {
			final String childId = scanner.next();
			MappedMutableTreeNode child = this.get(childId);
			if (child == null) {
				child = new MappedMutableTreeNode(childId);
				this.put(childId, child);
			}
			child.addNewTreeNode(scanner);
		}
	}

	public final String getId() {
		return id;
	}

}