//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEditor.IMGUI.Controls;
using UnityEngine;

namespace PopcornFX
{
	public static class TreeElementUtility
	{
		public static void TreeToList<T>(T root, IList<T> result) where T : TreeElement
		{
			if (result == null)
				throw new NullReferenceException("The input 'IList<T> result' list is null");
			result.Clear();

			Stack<T> stack = new Stack<T>();
			stack.Push(root);

			while (stack.Count > 0)
			{
				T current = stack.Pop();
				result.Add(current);

				if (current.children != null && current.children.Count > 0)
				{
					for (int i = current.children.Count - 1; i >= 0; i--)
					{
						stack.Push((T)current.children[i]);
					}
				}
			}
		}

		// Returns the root of the tree parsed from the list (always the first element).
		// Important: the first item and is required to have a depth value of -1. 
		// The rest of the items should have depth >= 0. 
		public static T ListToTree<T>(IList<T> list) where T : TreeElement
		{
			// Validate input
			ValidateDepthValues(list);

			// Clear old states
			foreach (var element in list)
			{
				element.parent = null;
				element.children = null;
			}

			// Set child and parent references using depth info
			for (int parentIndex = 0; parentIndex < list.Count; parentIndex++)
			{
				var parent = list[parentIndex];
				bool alreadyHasValidChildren = parent.children != null;
				if (alreadyHasValidChildren)
					continue;

				int parentDepth = parent.depth;
				int childCount = 0;

				// Count children based depth value, we are looking at children until it's the same depth as this object
				for (int i = parentIndex + 1; i < list.Count; i++)
				{
					if (list[i].depth == parentDepth + 1)
						childCount++;
					if (list[i].depth <= parentDepth)
						break;
				}

				// Fill child array
				List<TreeElement> childList = null;
				if (childCount != 0)
				{
					childList = new List<TreeElement>(childCount); // Allocate once
					childCount = 0;
					for (int i = parentIndex + 1; i < list.Count; i++)
					{
						if (list[i].depth == parentDepth + 1)
						{
							list[i].parent = parent;
							childList.Add(list[i]);
							childCount++;
						}

						if (list[i].depth <= parentDepth)
							break;
					}
				}

				parent.children = childList;
			}

			return list[0];
		}

		// Check state of input list
		public static void ValidateDepthValues<T>(IList<T> list) where T : TreeElement
		{
			if (list.Count == 0)
				throw new ArgumentException("list should have items, count is 0, check before calling ValidateDepthValues", "list");

			if (list[0].depth != -1)
				throw new ArgumentException("list item at index 0 should have a depth of -1 (since this should be the hidden root of the tree). Depth is: " + list[0].depth, "list");

			for (int i = 0; i < list.Count - 1; i++)
			{
				int depth = list[i].depth;
				int nextDepth = list[i + 1].depth;
				if (nextDepth > depth && nextDepth - depth > 1)
					throw new ArgumentException(string.Format("Invalid depth info in input list. Depth cannot increase more than 1 per row. Index {0} has depth {1} while index {2} has depth {3}", i, depth, i + 1, nextDepth));
			}

			for (int i = 1; i < list.Count; ++i)
				if (list[i].depth < 0)
					throw new ArgumentException("Invalid depth value for item at index " + i + ". Only the first item (the root) should have depth below 0.");

			if (list.Count > 1 && list[1].depth != 0)
				throw new ArgumentException("Input list item at index 1 is assumed to have a depth of 0", "list");
		}


		// For updating depth values below any given element e.g after reparenting elements
		public static void UpdateDepthValues<T>(T root) where T : TreeElement
		{
			if (root == null)
				throw new ArgumentNullException("root", "The root is null");

			if (!root.hasChildren)
				return;

			Stack<TreeElement> stack = new Stack<TreeElement>();
			stack.Push(root);
			while (stack.Count > 0)
			{
				TreeElement current = stack.Pop();
				if (current.children != null)
				{
					foreach (var child in current.children)
					{
						child.depth = current.depth + 1;
						stack.Push(child);
					}
				}
			}
		}

		// Returns true if there is an ancestor of child in the elements list
		static bool IsChildOf<T>(T child, IList<T> elements) where T : TreeElement
		{
			while (child != null)
			{
				child = (T)child.parent;
				if (elements.Contains(child))
					return true;
			}
			return false;
		}

		public static IList<T> FindCommonAncestorsWithinList<T>(IList<T> elements) where T : TreeElement
		{
			if (elements.Count == 1)
				return new List<T>(elements);

			List<T> result = new List<T>(elements);
			result.RemoveAll(g => IsChildOf(g, elements));
			return result;
		}
	}

	[Serializable]
	public class TreeElement
	{
		[SerializeField]
		int m_ID;
		[SerializeField]
		string m_Name;
		[SerializeField]
		int m_Depth;
		[NonSerialized]
		TreeElement m_Parent;
		[NonSerialized]
		protected List<TreeElement> m_Children;

		public int depth
		{
			get { return m_Depth; }
			set { m_Depth = value; }
		}

		public TreeElement parent
		{
			get { return m_Parent; }
			set { m_Parent = value; }
		}

		public List<TreeElement> children
		{
			get { return m_Children; }
			set { m_Children = value; }
		}

		public bool hasChildren
		{
			get { return children != null && children.Count > 0; }
		}

		public string name
		{
			get { return m_Name; }
			set { m_Name = value; }
		}

		public int id
		{
			get { return m_ID; }
			set { m_ID = value; }
		}

		public TreeElement()
		{
		}

		public TreeElement(string name, int depth, int id)
		{
			m_Name = name;
			m_ID = id;
			m_Depth = depth;
		}
	}

	public class TreeModel<T> where T : TreeElement
	{
		IList<T> m_Data;
		T m_Root;
		int m_MaxID;
		bool m_CanDrag = false;

		public T root { get { return m_Root; } set { m_Root = value; } }
		public event Action modelChanged;
		public int numberOfDataElements
		{
			get { return m_Data.Count; }
		}

		public TreeModel(IList<T> data)
		{
			SetData(data);
		}

		public T Find(int id)
		{
			return m_Data.FirstOrDefault(element => element.id == id);
		}

		public void SetData(IList<T> data)
		{
			Init(data);
		}

		void Init(IList<T> data)
		{
			if (data == null)
				throw new ArgumentNullException("data", "Input data is null. Ensure input is a non-null list.");

			m_Data = data;
			if (m_Data.Count > 0)
				m_Root = TreeElementUtility.ListToTree(data);

			m_MaxID = m_Data.Max(e => e.id);
		}

		public int GenerateUniqueID()
		{
			return ++m_MaxID;
		}

		public IList<int> GetAncestors(int id)
		{
			var parents = new List<int>();
			TreeElement T = Find(id);
			if (T != null)
			{
				while (T.parent != null)
				{
					parents.Add(T.parent.id);
					T = T.parent;
				}
			}
			return parents;
		}

		public IList<int> GetDescendantsThatHaveChildren(int id)
		{
			T searchFromThis = Find(id);
			if (searchFromThis != null)
			{
				return GetParentsBelowStackBased(searchFromThis);
			}
			return new List<int>();
		}

		IList<int> GetParentsBelowStackBased(TreeElement searchFromThis)
		{
			Stack<TreeElement> stack = new Stack<TreeElement>();
			stack.Push(searchFromThis);

			var parentsBelow = new List<int>();
			while (stack.Count > 0)
			{
				TreeElement current = stack.Pop();
				if (current.hasChildren)
				{
					parentsBelow.Add(current.id);
					foreach (var T in current.children)
					{
						stack.Push(T);
					}
				}
			}

			return parentsBelow;
		}

		public void RemoveElements(IList<int> elementIDs)
		{
			IList<T> elements = m_Data.Where(element => elementIDs.Contains(element.id)).ToArray();
			RemoveElements(elements);
		}

		public void RemoveElements(IList<T> elements)
		{
			foreach (var element in elements)
				if (element == m_Root)
					throw new ArgumentException("It is not allowed to remove the root element");

			var commonAncestors = TreeElementUtility.FindCommonAncestorsWithinList(elements);

			foreach (var element in commonAncestors)
			{
				element.parent.children.Remove(element);
				element.parent = null;
			}

			TreeElementUtility.TreeToList(m_Root, m_Data);

			Changed();
		}

		public void AddElements(IList<T> elements, TreeElement parent, int insertPosition)
		{
			if (elements == null)
				throw new ArgumentNullException("elements", "elements is null");
			if (elements.Count == 0)
				throw new ArgumentNullException("elements", "elements Count is 0: nothing to add");
			if (parent == null)
				throw new ArgumentNullException("parent", "parent is null");

			if (parent.children == null)
				parent.children = new List<TreeElement>();

			parent.children.InsertRange(insertPosition, elements.Cast<TreeElement>());
			foreach (var element in elements)
			{
				element.parent = parent;
				element.depth = parent.depth + 1;
				TreeElementUtility.UpdateDepthValues(element);
			}

			TreeElementUtility.TreeToList(m_Root, m_Data);

			Changed();
		}

		public void AddRoot(T root)
		{
			if (root == null)
				throw new ArgumentNullException("root", "root is null");

			if (m_Data == null)
				throw new InvalidOperationException("Internal Error: data list is null");

			if (m_Data.Count != 0)
				throw new InvalidOperationException("AddRoot is only allowed on empty data list");

			root.id = GenerateUniqueID();
			root.depth = -1;
			m_Data.Add(root);
		}

		public void AddElement(T element, TreeElement parent, int insertPosition)
		{
			if (element == null)
				throw new ArgumentNullException("element", "element is null");
			if (parent == null)
				throw new ArgumentNullException("parent", "parent is null");

			if (parent.children == null)
				parent.children = new List<TreeElement>();

			parent.children.Insert(insertPosition, element);
			element.parent = parent;

			TreeElementUtility.UpdateDepthValues(parent);
			TreeElementUtility.TreeToList(m_Root, m_Data);

			Changed();
		}

		public void MoveElements(TreeElement parentElement, int insertionIndex, List<TreeElement> elements)
		{
			if (!m_CanDrag)
				return;

			if (insertionIndex < 0)
				throw new ArgumentException("Invalid input: insertionIndex is -1, client needs to decide what index elements should be reparented at");

			// Invalid reparenting input
			if (parentElement == null)
				return;

			// We are moving items so we adjust the insertion index to accomodate that any items above the insertion index is removed before inserting
			if (insertionIndex > 0)
				insertionIndex -= parentElement.children.GetRange(0, insertionIndex).Count(elements.Contains);

			// Remove draggedItems from their parents
			foreach (var draggedItem in elements)
			{
				draggedItem.parent.children.Remove(draggedItem);            // remove from old parent
				draggedItem.parent = parentElement;                         // set new parent
			}

			if (parentElement.children == null)
				parentElement.children = new List<TreeElement>();

			// Insert dragged items under new parent
			parentElement.children.InsertRange(insertionIndex, elements);

			TreeElementUtility.UpdateDepthValues(root);
			TreeElementUtility.TreeToList(m_Root, m_Data);

			Changed();
		}

		void Changed()
		{
			if (modelChanged != null)
				modelChanged();
		}
	}

	internal class TreeViewItem<T> : TreeViewItem where T : TreeElement
	{
		public T data { get; set; }

		public TreeViewItem(int id, int depth, string displayName, T data) : base(id, depth, displayName)
		{
			this.data = data;
		}
	}

	internal class TreeViewWithTreeModel<T> : TreeView where T : TreeElement
	{
		TreeModel<T> m_TreeModel;
		readonly List<TreeViewItem> m_Rows = new List<TreeViewItem>(100);
		public event Action treeChanged;

		public TreeModel<T> treeModel { get { return m_TreeModel; } }

		public TreeViewWithTreeModel(TreeViewState state, TreeModel<T> model) : base(state)
		{
			Init(model);
		}

		public TreeViewWithTreeModel(TreeViewState state, MultiColumnHeader multiColumnHeader, TreeModel<T> model)
			: base(state, multiColumnHeader)
		{
			Init(model);
		}

		void Init(TreeModel<T> model)
		{
			m_TreeModel = model;
			m_TreeModel.modelChanged += ModelChanged;
		}

		void ModelChanged()
		{
			if (treeChanged != null)
				treeChanged();

			Reload();
		}

		protected override TreeViewItem BuildRoot()
		{
			int depthForHiddenRoot = -1;
			return new TreeViewItem<T>(m_TreeModel.root.id, depthForHiddenRoot, m_TreeModel.root.name, m_TreeModel.root);
		}

		protected override IList<TreeViewItem> BuildRows(TreeViewItem root)
		{
			if (m_TreeModel.root == null)
			{
				Debug.LogError("[PKFX] Tree model root is null. did you call SetData()?");
			}

			m_Rows.Clear();
			if (!string.IsNullOrEmpty(searchString))
			{
				Search(m_TreeModel.root, searchString, m_Rows);
			}
			else
			{
				if (m_TreeModel.root.hasChildren)
					AddChildrenRecursive(m_TreeModel.root, 0, m_Rows);
			}

			// We still need to setup the child parent information for the rows since this 
			// information is used by the TreeView internal logic (navigation, dragging etc)
			SetupParentsAndChildrenFromDepths(root, m_Rows);

			return m_Rows;
		}

		void AddChildrenRecursive(T parent, int depth, IList<TreeViewItem> newRows)
		{
			foreach (T child in parent.children)
			{
				var item = new TreeViewItem<T>(child.id, depth, child.name, child);
				newRows.Add(item);

				if (child.hasChildren)
				{
					if (IsExpanded(child.id))
					{
						AddChildrenRecursive(child, depth + 1, newRows);
					}
					else
					{
						item.children = CreateChildListForCollapsedParent();
					}
				}
			}
		}

		void Search(T searchFromThis, string search, List<TreeViewItem> result)
		{
			if (string.IsNullOrEmpty(search))
				throw new ArgumentException("Invalid search: cannot be null or empty", "search");

			const int kItemDepth = 0; // tree is flattened when searching

			Stack<T> stack = new Stack<T>();
			foreach (var element in searchFromThis.children)
				stack.Push((T)element);
			while (stack.Count > 0)
			{
				T current = stack.Pop();
				// Matches search?
				if (current.name.IndexOf(search, StringComparison.OrdinalIgnoreCase) >= 0)
				{
					result.Add(new TreeViewItem<T>(current.id, kItemDepth, current.name, current));
				}

				if (current.children != null && current.children.Count > 0)
				{
					foreach (var element in current.children)
					{
						stack.Push((T)element);
					}
				}
			}
			SortSearchResult(result);
		}

		protected virtual void SortSearchResult(List<TreeViewItem> rows)
		{
			rows.Sort((x, y) => EditorUtility.NaturalCompare(x.displayName, y.displayName)); // sort by displayName by default, can be overriden for multicolumn solutions
		}

		protected override IList<int> GetAncestors(int id)
		{
			return m_TreeModel.GetAncestors(id);
		}

		protected override IList<int> GetDescendantsThatHaveChildren(int id)
		{
			return m_TreeModel.GetDescendantsThatHaveChildren(id);
		}

		/*
		// Dragging
		//-----------
		public event Action<IList<TreeViewItem>> beforeDroppingDraggedItems;
		const string k_GenericDragID = "GenericDragColumnDragging";

		protected override bool CanStartDrag(CanStartDragArgs args)
		{
			return true;
		}

		protected override void SetupDragAndDrop(SetupDragAndDropArgs args)
		{
			if (hasSearch)
				return;

			DragAndDrop.PrepareStartDrag();
			var draggedRows = GetRows().Where(item => args.draggedItemIDs.Contains(item.id)).ToList();
			DragAndDrop.SetGenericData(k_GenericDragID, draggedRows);
			DragAndDrop.objectReferences = new UnityEngine.Object[] { }; // this IS required for dragging to work
			string title = draggedRows.Count == 1 ? draggedRows[0].displayName : "< Multiple >";
			DragAndDrop.StartDrag(title);
		}

		protected override DragAndDropVisualMode HandleDragAndDrop(DragAndDropArgs args)
		{
			// Check if we can handle the current drag data (could be dragged in from other areas/windows in the editor)
			var draggedRows = DragAndDrop.GetGenericData(k_GenericDragID) as List<TreeViewItem>;
			if (draggedRows == null)
				return DragAndDropVisualMode.None;

			// Parent item is null when dragging outside any tree view items.
			switch (args.dragAndDropPosition)
			{
				case DragAndDropPosition.UponItem:
				case DragAndDropPosition.BetweenItems:
					{
						bool validDrag = ValidDrag(args.parentItem, draggedRows);
						if (args.performDrop && validDrag)
						{
							T parentData = ((TreeViewItem<T>)args.parentItem).data;
							OnDropDraggedElementsAtIndex(draggedRows, parentData, args.insertAtIndex == -1 ? 0 : args.insertAtIndex);
						}
						return validDrag ? DragAndDropVisualMode.Move : DragAndDropVisualMode.None;
					}

				case DragAndDropPosition.OutsideItems:
					{
						if (args.performDrop)
							OnDropDraggedElementsAtIndex(draggedRows, m_TreeModel.root, m_TreeModel.root.children.Count);

						return DragAndDropVisualMode.Move;
					}
				default:
					Debug.LogError("Unhandled enum " + args.dragAndDropPosition);
					return DragAndDropVisualMode.None;
			}
		}

		public virtual void OnDropDraggedElementsAtIndex(List<TreeViewItem> draggedRows, T parent, int insertIndex)
		{
			if (beforeDroppingDraggedItems != null)
				beforeDroppingDraggedItems(draggedRows);

			var draggedElements = new List<TreeElement>();
			foreach (var x in draggedRows)
				draggedElements.Add(((TreeViewItem<T>)x).data);

			var selectedIDs = draggedElements.Select(x => x.id).ToArray();
			m_TreeModel.MoveElements(parent, insertIndex, draggedElements);
			SetSelection(selectedIDs, TreeViewSelectionOptions.RevealAndFrame);
		}


		bool ValidDrag(TreeViewItem parent, List<TreeViewItem> draggedItems)
		{
			TreeViewItem currentParent = parent;
			while (currentParent != null)
			{
				if (draggedItems.Contains(currentParent))
					return false;
				currentParent = currentParent.parent;
			}
			return true;
		}
		*/
	}
}