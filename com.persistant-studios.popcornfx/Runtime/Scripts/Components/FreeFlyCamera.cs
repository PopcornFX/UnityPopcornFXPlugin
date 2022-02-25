//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

public class InputManagerEntry
{
	public enum Kind	{ KeyOrButton, Mouse, Axis }
	public enum Axis	{ X, Y, Third, Fourth, Fifth, Sixth, Seventh, Eigth }
	public enum Joy		{ All, First, Second }

	public string	name = "";
	public string	desc = "";
	public string	btnNegative = "";
	public string	btnPositive = "";
	public string	altBtnNegative = "";
	public string	altBtnPositive = "";
	public float	gravity = 0.0f;
	public float	deadZone = 0.0f;
	public float	sensitivity = 0.0f;
	public bool		snap = false;
	public bool		invert = false;
	public Kind		kind = Kind.Axis;
	public Axis		axis = Axis.X;
	public Joy		joystick = Joy.All;
}

public class InputRegistering
{
	static bool InputAlreadyRegistered(string name, InputManagerEntry.Kind kind, SerializedProperty spAxes)
	{
		for (var i = 0; i < spAxes.arraySize; ++i)
		{
			var spAxis = spAxes.GetArrayElementAtIndex(i);
			var axisName = spAxis.FindPropertyRelative("m_Name").stringValue;
			var kindValue = spAxis.FindPropertyRelative("type").intValue;
			if (axisName == name && (int)kind == kindValue)
				return true;
		}

		return false;
	}

	static void WriteEntry(SerializedProperty spAxes, InputManagerEntry entry)
	{
		if (InputAlreadyRegistered(entry.name, entry.kind, spAxes))
			return;

		spAxes.InsertArrayElementAtIndex(spAxes.arraySize);
		var spAxis = spAxes.GetArrayElementAtIndex(spAxes.arraySize - 1);
		spAxis.FindPropertyRelative("m_Name").stringValue = entry.name;
		spAxis.FindPropertyRelative("descriptiveName").stringValue = entry.desc;
		spAxis.FindPropertyRelative("negativeButton").stringValue = entry.btnNegative;
		spAxis.FindPropertyRelative("altNegativeButton").stringValue = entry.altBtnNegative;
		spAxis.FindPropertyRelative("positiveButton").stringValue = entry.btnPositive;
		spAxis.FindPropertyRelative("altPositiveButton").stringValue = entry.altBtnPositive;
		spAxis.FindPropertyRelative("gravity").floatValue = entry.gravity;
		spAxis.FindPropertyRelative("dead").floatValue = entry.deadZone;
		spAxis.FindPropertyRelative("sensitivity").floatValue = entry.sensitivity;
		spAxis.FindPropertyRelative("snap").boolValue = entry.snap;
		spAxis.FindPropertyRelative("invert").boolValue = entry.invert;
		spAxis.FindPropertyRelative("type").intValue = (int)entry.kind;
		spAxis.FindPropertyRelative("axis").intValue = (int)entry.axis;
		spAxis.FindPropertyRelative("joyNum").intValue = (int)entry.joystick;
	}

	public static void RegisterInputs(List<InputManagerEntry> entries)
	{
		// Grab reference to input manager
		var assets = AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/InputManager.asset");
		// Temporary fix. This happens some time with HDRP init when it's called before asset database is initialized (probably related to package load order).
		if (assets.Length == 0)
			return;

		var inputManager = assets[0];

		// Wrap in serialized object
		var soInputManager = new SerializedObject(inputManager);
		var spAxes = soInputManager.FindProperty("m_Axes");

		foreach (InputManagerEntry entry in entries)
		{
			WriteEntry(spAxes, entry);
		}

		// Commit
		soInputManager.ApplyModifiedProperties();
	}
}
#endif

[ExecuteAlways]
public class FreeFlyCamera : MonoBehaviour
{
	public float m_LookSpeedController = 120f;
	public float m_LookSpeedMouse = 10.0f;
	public float m_MoveSpeed = 10.0f;
	public float m_MoveSpeedIncrement = 2.5f;
	public float m_Turbo = 10.0f;

	private static string kMouseX = "Mouse X";
	private static string kMouseY = "Mouse Y";
	private static string kRightStickX = "Controller Right Stick X";
	private static string kRightStickY = "Controller Right Stick Y";
	private static string kVertical = "Vertical";
	private static string kHorizontal = "Horizontal";

	private static string kYAxis = "YAxis";
	private static string kSpeedAxis = "Speed Axis";

	void OnEnable()
	{
		RegisterInputs();
	}

	void RegisterInputs()
	{
#if UNITY_EDITOR
		List<InputManagerEntry> inputEntries = new List<InputManagerEntry>();

		// Add new bindings
		inputEntries.Add(new InputManagerEntry { name = kRightStickX, kind = InputManagerEntry.Kind.Axis, axis = InputManagerEntry.Axis.Fourth, sensitivity = 1.0f, gravity = 1.0f, deadZone = 0.2f });
		inputEntries.Add(new InputManagerEntry { name = kRightStickY, kind = InputManagerEntry.Kind.Axis, axis = InputManagerEntry.Axis.Fifth, sensitivity = 1.0f, gravity = 1.0f, deadZone = 0.2f, invert = true });

		inputEntries.Add(new InputManagerEntry { name = kYAxis, kind = InputManagerEntry.Kind.KeyOrButton, btnPositive = "page up", altBtnPositive = "joystick button 5", btnNegative = "page down", altBtnNegative = "joystick button 4", gravity = 1000.0f, deadZone = 0.001f, sensitivity = 1000.0f });

		inputEntries.Add(new InputManagerEntry { name = kSpeedAxis, kind = InputManagerEntry.Kind.KeyOrButton, btnPositive = "home", btnNegative = "end", gravity = 1000.0f, deadZone = 0.001f, sensitivity = 1000.0f });
		inputEntries.Add(new InputManagerEntry { name = kSpeedAxis, kind = InputManagerEntry.Kind.Axis, axis = InputManagerEntry.Axis.Seventh, gravity = 1000.0f, deadZone = 0.001f, sensitivity = 1000.0f });

		InputRegistering.RegisterInputs(inputEntries);
#endif
	}

	void Update()
	{
		float inputRotateAxisX = 0.0f;
		float inputRotateAxisY = 0.0f;
		if (Input.GetMouseButton(1))
		{
			inputRotateAxisX = Input.GetAxis(kMouseX) * m_LookSpeedMouse;
			inputRotateAxisY = Input.GetAxis(kMouseY) * m_LookSpeedMouse;
		}
		inputRotateAxisX += (Input.GetAxis(kRightStickX) * m_LookSpeedController * Time.deltaTime);
		inputRotateAxisY += (Input.GetAxis(kRightStickY) * m_LookSpeedController * Time.deltaTime);

		float inputChangeSpeed = Input.GetAxis(kSpeedAxis);
		if (inputChangeSpeed != 0.0f)
		{
			m_MoveSpeed += inputChangeSpeed * m_MoveSpeedIncrement;
			if (m_MoveSpeed < m_MoveSpeedIncrement) m_MoveSpeed = m_MoveSpeedIncrement;
		}

		float inputVertical = Input.GetAxis(kVertical);
		float inputHorizontal = Input.GetAxis(kHorizontal);
		float inputYAxis = Input.GetAxis(kYAxis);

		bool moved = inputRotateAxisX != 0.0f || inputRotateAxisY != 0.0f || inputVertical != 0.0f || inputHorizontal != 0.0f || inputYAxis != 0.0f;
		if (moved)
		{
			float rotationX = transform.localEulerAngles.x;
			float newRotationY = transform.localEulerAngles.y + inputRotateAxisX;

			// Weird clamping code due to weird Euler angle mapping...
			float newRotationX = (rotationX - inputRotateAxisY);
			if (rotationX <= 90.0f && newRotationX >= 0.0f)
				newRotationX = Mathf.Clamp(newRotationX, 0.0f, 90.0f);
			if (rotationX >= 270.0f)
				newRotationX = Mathf.Clamp(newRotationX, 270.0f, 360.0f);

			transform.localRotation = Quaternion.Euler(newRotationX, newRotationY, transform.localEulerAngles.z);

			float moveSpeed = Time.deltaTime * m_MoveSpeed;
			if (Input.GetMouseButton(1))
				moveSpeed *= Input.GetKey(KeyCode.LeftShift) ? m_Turbo : 1.0f;
			else
				moveSpeed *= Input.GetAxis("Fire1") > 0.0f ? m_Turbo : 1.0f;
			transform.position += transform.forward * moveSpeed * inputVertical;
			transform.position += transform.right * moveSpeed * inputHorizontal;
			transform.position += Vector3.up * moveSpeed * inputYAxis;
		}
	}
}

