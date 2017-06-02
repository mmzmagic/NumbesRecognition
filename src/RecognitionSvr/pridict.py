from __future__ import absolute_import
from __future__ import print_function
from keras.models import model_from_json
import numpy as np
import sys
from PIL import Image

np.random.seed(1337)  # for reproducibility
import keras

STANDARD_SIZE = (28, 28)

def get_image_data(ifile):
    img = Image.open(ifile)
    #img = alpha_to_color(img)
    #img = trim(img)
    img = img.convert('L')
    img = img.getdata()
    img = img.resize(STANDARD_SIZE)
    #img = np.array(img)/255.
    #img = [1-i for i in img]
    return img

def pridict(ifile):
    data = np.empty((1,28,28,1),dtype="float32")  
    img = Image.open(ifile)
    img = img.resize(STANDARD_SIZE)
    img=np.asarray(img)
    
    ll = list()
    ll.append(img)
    xx = np.array(ll)
    ll2 = list()
    ll2.append(xx)
    xx2 = np.array(ll2) 
    predict = model.predict(xx2, batch_size=1, verbose=1)
    return np.argmax(predict[0])


def initcnn():
    model = model_from_json(open('my_cnn_model_json_str.json').read())
    model.load_weights('my_cnn_model_weights.h5')
    # dont forget to compile your model
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

if len(sys.argv[0])>1:
   print(pridict(sys.argv[1]))
