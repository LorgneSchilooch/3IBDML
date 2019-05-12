using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class MainScript : MonoBehaviour
{

	[DllImport(@"/Assets/lib2018_3A_IBD.dylib")]
	public static extern int return42();
	// Use this for initialization
	void Start () {
		Debug.Log(return42());
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
