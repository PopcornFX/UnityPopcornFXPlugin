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
	[Serializable]
	//The TreeElement data class is extended to hold extra data, which you can show and edit in the front-end TreeView.
	internal class PKFxAssetElement : TreeElement
	{
		[SerializeField]
		bool m_Import;
		public bool Import
		{
			get { return m_Import; }
			set
			{
				if (m_Import != value)
				{
					m_Import = value;
					if (m_Children == null)
						return;
					foreach (PKFxAssetElement child in m_Children)
					{
						child.Import = value;
					}
				}

			}
		}

		[SerializeField]
		string m_FullPath;
		public string FullPath
		{
			get { return m_FullPath; }
			set { m_FullPath = value; }
		}

		public PKFxAssetElement(string name, int depth, int id, string path, bool import)
			: base(name, depth, id)
		{
			m_FullPath = path;
			m_Import = import;
		}
	}

	class PKFXAssetTreeView : TreeViewWithTreeModel<PKFxAssetElement>
	{
		const float kToggleWidth = 18f;
		//columns
		enum Columns
		{
			Name,
		}

		public PKFXAssetTreeView(TreeViewState state, MultiColumnHeader multicolumnHeader, TreeModel<PKFxAssetElement> model)
			: base(state, multicolumnHeader, model)
		{
			showBorder = true;
			showAlternatingRowBackgrounds = true;

			getNewSelectionOverride = SelectionOverride;
		}


		protected override void RowGUI(RowGUIArgs args)
		{
			var item = (TreeViewItem<PKFxAssetElement>)args.item;

			for (int i = 0; i < args.GetNumVisibleColumns(); ++i)
			{
				CellGUI(args.GetCellRect(i), item, (Columns)args.GetColumn(i), ref args);
			}
		}

		public List<int> SelectionOverride(TreeViewItem clickedItem, bool keepMultiSelection, bool useActionKeyAsShift)
		{
			TreeViewItem<PKFxAssetElement> item = clickedItem as TreeViewItem<PKFxAssetElement>;
			if (item != null && keepMultiSelection == false)
			{
				item.data.Import = !item.data.Import;
			}
			return new List<int>();
		}

		void CellGUI(Rect cellRect, TreeViewItem<PKFxAssetElement> item, Columns column, ref RowGUIArgs args)
		{
			// Center cell rect vertically (makes it easier to place controls, icons etc in the cells)
			CenterRectUsingSingleLineHeight(ref cellRect);

			switch (column)
			{
				case Columns.Name:
					{
						// Do toggle
						Rect toggleRect = cellRect;
						toggleRect.x += GetContentIndent(item);
						toggleRect.width = kToggleWidth;
						if (toggleRect.xMax < cellRect.xMax)
							item.data.Import = EditorGUI.Toggle(toggleRect, item.data.Import); // hide when outside cell rect

						cellRect.x += kToggleWidth;
						cellRect.width -= kToggleWidth;
						// Default icon and label
						args.rowRect = cellRect;
						base.RowGUI(args);
					}
					break;
				default:
					break;
			}
		}

		public static MultiColumnHeaderState CreateDefaultMultiColumnHeaderState(float treeViewWidth)
		{
			var columns = new[]
			{
				new MultiColumnHeaderState.Column
				{
					headerContent = new GUIContent("Name"),
					headerTextAlignment = TextAlignment.Left,
					sortedAscending = true,
					sortingArrowAlignment = TextAlignment.Center,
					width = 150,
					minWidth = 60,
					autoResize = true,
					allowToggleVisibility = false
				},
			};

			MultiColumnHeaderState state = new MultiColumnHeaderState(columns);
			return state;
		}

		public List<string> GetSelectedAssetPath()
		{
			List<string> paths = new List<string>();
			List<PKFxAssetElement> elements = new List<PKFxAssetElement>();

			TreeElementUtility.TreeToList<PKFxAssetElement>(treeModel.root, elements);

			foreach (PKFxAssetElement element in elements)
			{
				if (element.Import && element.FullPath != null)
					paths.Add(element.FullPath);
			}
			return paths;
		}
	}


	internal class PKFxMultiColumnHeader : MultiColumnHeader
	{
		Mode m_Mode;

		public enum Mode
		{
			LargeHeader,
			DefaultHeader,
			MinimumHeaderWithoutSorting
		}

		public PKFxMultiColumnHeader(MultiColumnHeaderState state)
			: base(state)
		{
			mode = Mode.DefaultHeader;
		}

		public Mode mode
		{
			get
			{
				return m_Mode;
			}
			set
			{
				m_Mode = value;
				switch (m_Mode)
				{
					case Mode.LargeHeader:
						canSort = true;
						height = 37f;
						break;
					case Mode.DefaultHeader:
						canSort = true;
						height = DefaultGUI.defaultHeight;
						break;
					case Mode.MinimumHeaderWithoutSorting:
						canSort = false;
						height = DefaultGUI.minimumHeight;
						break;
				}
			}
		}

		protected override void ColumnHeaderGUI(MultiColumnHeaderState.Column column, Rect headerRect, int columnIndex)
		{
			// Default column header gui
			base.ColumnHeaderGUI(column, headerRect, columnIndex);

			// Add additional info for large header
			if (mode == Mode.LargeHeader)
			{
				// Show example overlay stuff on some of the columns
				if (columnIndex > 2)
				{
					headerRect.xMax -= 3f;
					var oldAlignment = EditorStyles.largeLabel.alignment;
					EditorStyles.largeLabel.alignment = TextAnchor.UpperRight;
					GUI.Label(headerRect, 36 + columnIndex + "%", EditorStyles.largeLabel);
					EditorStyles.largeLabel.alignment = oldAlignment;
				}
			}
		}
	}

	public class ImportPKFxListEditor : EditorWindow
	{
		[NonSerialized]
		bool m_Initialized;
		[SerializeField]
		TreeViewState m_TreeViewState;
		[SerializeField]
		MultiColumnHeaderState m_MultiColumnHeaderState;

		PKFXAssetTreeView m_FxTreeView;

		SearchField m_SearchField;

		private List<string> m_Paths;
		public List<string> Paths
		{
			get { return m_Paths; }
			set { m_Paths = value; }
		}

		Rect multiColumnTreeViewRect
		{
			get { return new Rect(0, 30, position.width, position.height - 60); }
		}

		Rect bottomToolbarRect
		{
			get { return new Rect(10, position.height - 23f, position.width - 30f, 16f); }
		}

		public ImportPKFxListEditor()
		{
		}

		private TreeModel<PKFxAssetElement> GetTreeModelFromList()
		{
			List<PKFxAssetElement> elementList = new List<PKFxAssetElement>();
			int id = 0;

			elementList.Add(new PKFxAssetElement("PopcornFX", -1, id, null, false));
			if (m_Paths == null)
				return new TreeModel<PKFxAssetElement>(elementList);

			foreach (string path in m_Paths)
			{
				string[] splittedPath = path.Split('/');

				int depth = 0;
				int length = splittedPath.Length;
				for (int i = 0; i < length; ++i)
				{
					string element = splittedPath[i];
					id++;
					if (i < length - 1) //Directory
					{
						if (!(elementList.Find(x => (x.name == element && x.depth == depth)) != null))
						{
							elementList.Add(new PKFxAssetElement(element, depth, id, null, false));
						}
					}
					else // Effect
					{
						elementList.Add(new PKFxAssetElement(element, depth, id, path, false));
					}
					++depth;
				}
			}
			return new TreeModel<PKFxAssetElement>(elementList);
		}

		private void OnEnable()
		{
			m_Paths = PKFxSettings.AssetPathList;
			if (m_Paths == null || m_Paths.Count == 0)
			{
				Debug.LogWarning("[PopcornFX] Source Pack path is required to import your FXs", this);
				return;
			}
			InitializeIFN();
		}

		private void InitializeIFN()
		{
			if (!m_Initialized)
			{
				// Check if it already exists (deserialized from window layout file or scriptable object)
				if (m_TreeViewState == null)
					m_TreeViewState = new TreeViewState();

				bool firstInit = m_MultiColumnHeaderState == null;

				var headerState = PKFXAssetTreeView.CreateDefaultMultiColumnHeaderState(multiColumnTreeViewRect.width);
				if (MultiColumnHeaderState.CanOverwriteSerializedFields(m_MultiColumnHeaderState, headerState))
					MultiColumnHeaderState.OverwriteSerializedFields(m_MultiColumnHeaderState, headerState);
				m_MultiColumnHeaderState = headerState;

				PKFxMultiColumnHeader multiColumnHeader = new PKFxMultiColumnHeader(headerState);

				multiColumnHeader.ResizeToFit();

				TreeModel<PKFxAssetElement> treeModel = GetTreeModelFromList();

				m_FxTreeView = new PKFXAssetTreeView(m_TreeViewState, multiColumnHeader, treeModel);

				m_FxTreeView.Reload();
				m_FxTreeView.ExpandAll();

				m_SearchField = new SearchField();
				m_SearchField.downOrUpArrowKeyPressed += m_FxTreeView.SetFocusAndEnsureSelectedItem;
				m_Initialized = true;
			}
		}

		public void OnGUI()
		{
			InitializeIFN();
			ToolBar();

			DoTreeView(multiColumnTreeViewRect);
			BottomToolBar(bottomToolbarRect);
		}

		void DoTreeView(Rect rect)
		{
			m_FxTreeView.OnGUI(rect);
		}

		void ToolBar()
		{
			GUILayout.Space(5f);
			using (new EditorGUILayout.HorizontalScope())
			{
				var style = "miniButton";
				if (GUILayout.Button("Expand All", style))
				{
					m_FxTreeView.ExpandAll();
				}
				if (GUILayout.Button("Collapse All", style))
				{
					m_FxTreeView.CollapseAll();
				}
				GUILayout.FlexibleSpace();

				m_FxTreeView.searchString = m_SearchField.OnGUI(m_FxTreeView.searchString);
			}
			GUILayout.Space(3f);
		}

		void BottomToolBar(Rect rect)
		{
			GUILayout.BeginArea(rect);

			using (new EditorGUILayout.HorizontalScope())
			{
				var style = "miniButton";
				if (GUILayout.Button("Cancel", style))
				{
					Close();
				}
				GUILayout.FlexibleSpace();
				if (GUILayout.Button("Import Selected", style))
				{
					List<string> path = m_FxTreeView.GetSelectedAssetPath();
					PKFxSettings.ReimportAssets(path, PKFxSettings.CurrentPlatformName);
					Close();
				}
			}
			GUILayout.EndArea();
		}

		void OnHierarchyChange()
		{
			if (m_FxTreeView != null)
				m_FxTreeView.Reload();
			Repaint();
		}

		void OnSelectionChange()
		{
			if (m_FxTreeView != null)
				m_FxTreeView.SetSelection(Selection.instanceIDs);
			Repaint();
		}
	}
}