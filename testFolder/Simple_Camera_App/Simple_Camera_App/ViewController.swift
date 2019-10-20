//
//  ViewController.swift
//  Simple_Camera_App
//
//  Created by Stewart Vohra on 10/18/19.
//  Copyright © 2019 Stewart Vohra. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UIImagePickerControllerDelegate, UINavigationControllerDelegate
{

    override func viewDidLoad()
    {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        if !UIImagePickerController.isSourceTypeAvailable(UIImagePickerController.SourceType.camera)
        {
            let alert = UIAlertController(title: "My Alert", message: "Camera not avaliable on this device. ", preferredStyle: .alert)
            self.present(alert, animated: true, completion: nil)
        } else if !UIImagePickerController.isSourceTypeAvailable(UIImagePickerController.SourceType.savedPhotosAlbum)
        {
            let alert = UIAlertController(title: "My Alert", message: "Saved photos album not avaliable on this device. ", preferredStyle: .alert)
            self.present(alert, animated: true, completion: nil)
        }
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        self.navigationController?.setToolbarHidden(true, animated: true)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        self.navigationController?.setToolbarHidden(false, animated: true)
    }

    @IBAction func selectPhoto(_ sender: UIButton)
    {
        let pickerController = UIImagePickerController()
        pickerController.delegate = self
        pickerController.sourceType = .savedPhotosAlbum
        present(pickerController, animated: true)
    }
    
    
    @IBAction func takePhoto(_ sender: UIButton)
    {
        let pickerController = UIImagePickerController()
        pickerController.delegate = self
        pickerController.sourceType = .camera
        pickerController.cameraCaptureMode = .photo
        present(pickerController, animated: true)
    }
   
    @IBOutlet var imageView: UIImageView!
    
    // MARK: - UIImagePickerControllerDelegate
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any])
    {
        
        // The info dictionary may contain multiple representations of the image. You want to use the original.
        guard let selectedImage = info[UIImagePickerController.InfoKey.originalImage] as? UIImage else {
            fatalError("Expected a dictionary containing an image, but was provided the following: \(info)")
        }
        imageView.image = selectedImage
        // Dismiss the picker.
        dismiss(animated: true, completion: nil)
    }
    
    func imagePickerControllerDidCancel(_ picker: UIImagePickerController)
    {
        // Dismiss the picker.
        dismiss(animated: true, completion: nil)
    }
}

