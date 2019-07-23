import numpy as np
from keras.layers import Input, Conv2D, BatchNormalization, MaxPooling2D, AveragePooling2D, Flatten, Add, Dense, Activation
from keras.layers import ZeroPadding2D,Dropout
from keras.models import Model, load_model
from keras import optimizers
from keras.initializers import glorot_uniform
import matplotlib.pyplot as plt
import keras.backend as K
import h5py

K.set_image_data_format('channels_last')

def identity_block(X,f,filters,stage,block):
    conv_name_base= 'conv_s'+str(stage)+block+'_branch'
    bn_name_base = 'bn_s'+str(stage)+block+'_branch'

    F1,F2 = filters

    X_identity = X

    X = Conv2D(filters=F1, kernel_size=(f, f), strides=(1, 1), padding='same', name=conv_name_base + '2a',kernel_initializer=glorot_uniform(seed=0))(X)
    X = BatchNormalization(axis=3, name=bn_name_base + '2a')(X)
    X = Activation(activation='relu')(X)

    X = Conv2D(filters = F2,kernel_size=(f,f),strides=(1,1),padding='same',name = conv_name_base+'2b', kernel_initializer=glorot_uniform(seed=0))(X)
    X = BatchNormalization(axis=3, name = bn_name_base+'2b')(X)

    X = Add()([X_identity,X])
    X = Activation(activation='relu')(X)

    return X


def projection_block(X, f, filters, stage, block, s = 2):

    conv_name_base = 'conv_s' + str(stage) + block + '_branch'
    bn_name_base = 'bn_s' + str(stage) + block + '_branch'

    F1, F2 = filters

    X_projection = X

    X = Conv2D(filters=F1, kernel_size=(f, f), strides=(s,s), padding='same', name=conv_name_base + '2a',
               kernel_initializer=glorot_uniform(seed=0))(X)
    X = BatchNormalization(axis=3, name=bn_name_base + '2a')(X)
    X = Activation(activation='relu')(X)

    X = Conv2D(filters=F2, kernel_size=(f, f), strides=(1, 1), padding='same', name=conv_name_base + '2b',
               kernel_initializer=glorot_uniform(seed=0))(X)
    X = BatchNormalization(axis=3, name=bn_name_base + '2b')(X)

    X_projection = Conv2D(filters=F2, kernel_size=(f, f), strides=(s, s), padding='same', name=conv_name_base + '1',
               kernel_initializer=glorot_uniform(seed=0))(X_projection)
    X_projection = BatchNormalization(axis=3, name=bn_name_base + '1')(X_projection)

    X = Add()([X_projection, X])
    X = Activation(activation='relu')(X)

    return X


def resnet50_model(input_shape=(64,64,3), classes = 6):
    X_input = Input(input_shape)

    #stage 1
    X = ZeroPadding2D(padding=(3,3))(X_input)
    X = Conv2D(filters=64, kernel_size=(7,7), strides=(2,2),name='conv1', kernel_initializer=glorot_uniform(seed=0))(X)
    X = BatchNormalization(axis=3,name='bn_conv1')(X)
    X = Activation('relu')(X)
    X = MaxPooling2D(pool_size=(3,3),strides=(2,2))(X)

    #stage 2
    X = projection_block(X,f=3, filters=[64,64],stage=2,block='a',s=1)
    X = identity_block(X,3,[64,64],stage=2,block='b')
    X = identity_block(X,3,[64,64],stage=2,block='c')

    #stage 3
    X = projection_block(X,f=3, filters=[128,128],stage=3,block='a',s=2)
    X = identity_block(X,3,[128,128],stage=3,block='b')
    X = identity_block(X,3,[128,128],stage=3,block='c')
    X = identity_block(X, 3, [128,128], stage=3, block='d')

    #stage 4
    X = projection_block(X,f=3, filters=[256, 256],stage=4,block='a',s=2)
    X = identity_block(X,3,[256, 256],stage=4,block='b')
    X = identity_block(X,3,[256, 256],stage=4,block='c')
    X = identity_block(X,3, [256, 256], stage=4, block='d')
    X = identity_block(X,3,[256, 256],stage=4,block='e')
    X = identity_block(X, 3, [256, 256], stage=4, block='f')

    #stage 5
    X = projection_block(X,f=3, filters=[512, 512],stage=5,block='a',s=2)
    X = identity_block(X,3,[512, 512],stage=5,block='b')
    X = identity_block(X,3,[512, 512],stage=5,block='c')

    X = AveragePooling2D(pool_size=(2,2))(X)

    X = Flatten()(X)
    #X = Dropout(rate=0.5)(X)
    X = Dense(classes , activation='softmax', name='fc'+str(classes), kernel_initializer=glorot_uniform(seed=0))(X)

    model = Model(inputs=X_input, outputs=X, name='resnet50')

    return model

def load_dataset():
    train_dataset = h5py.File('datasets/train_signs.h5', "r")
    train_set_x_orig = np.array(train_dataset["train_set_x"][:])  # your train set features
    train_set_y_orig = np.array(train_dataset["train_set_y"][:])  # your train set labels

    test_dataset = h5py.File('datasets/test_signs.h5', "r")
    test_set_x_orig = np.array(test_dataset["test_set_x"][:])  # your test set features
    test_set_y_orig = np.array(test_dataset["test_set_y"][:])  # your test set labels

    classes = np.array(test_dataset["list_classes"][:])  # the list of classes
    train_set_y_orig = train_set_y_orig.reshape((1, train_set_y_orig.shape[0]))
    test_set_y_orig = test_set_y_orig.reshape((1, test_set_y_orig.shape[0]))

    return train_set_x_orig, train_set_y_orig, test_set_x_orig, test_set_y_orig, classes

def convert_to_one_hot(Y, C):
    Y = np.eye(C)[Y.reshape(-1)].T
    return Y

X_train_orig, Y_train_orig, X_test_orig, Y_test_orig, classes = load_dataset()
# Normalize image vectors
X_train = X_train_orig / 255.
X_test = X_test_orig / 255.

# Convert training and test labels to one hot matrices
Y_train = convert_to_one_hot(Y_train_orig, 6).T
Y_test = convert_to_one_hot(Y_test_orig, 6).T

print("number of training examples = " + str(X_train.shape[0]))
print("number of test examples = " + str(X_test.shape[0]))
print("X_train shape: " + str(X_train.shape))
print("Y_train shape: " + str(Y_train.shape))
print("X_test shape: " + str(X_test.shape))
print("Y_test shape: " + str(Y_test.shape))

rmodel = resnet50_model(input_shape=(64,64,3),classes=6)
opt = optimizers.Adam(lr=0.001)
rmodel.compile(optimizer=opt, loss='categorical_crossentropy', metrics=['accuracy'])
history = rmodel.fit(X_train,Y_train,validation_data=(X_test,Y_test),epochs=30,batch_size=64)

#evaluate
print(history.history)
print(rmodel.metrics_names)
preds = rmodel.evaluate(X_test, Y_test,batch_size=64)
print("Loss = " + str(preds[0]))
print("Test Accuracy = " + str(preds[1]))


plt.plot(history.history['acc'], label='train')
plt.plot(history.history['val_acc'], label='val')
plt.legend()
plt.show()

#model display
#rmodel.summary()

#official_model = load_model('resnet50')


