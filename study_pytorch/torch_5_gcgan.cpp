//
// Created by linwei on 2019-07-12.
//
#include <iostream>
#include <torch/torch.h>

using namespace torch;

const int kNoiseSize = 100;

nn::Sequential generator(
        // Layer 1
        nn::Conv2d(nn::Conv2dOptions(kNoiseSize, 256, 4)
                           .with_bias(false)
                           .transposed(true)),
        nn::BatchNorm(256),
        nn::Functional(torch::relu),
        // Layer 2
        nn::Conv2d(nn::Conv2dOptions(256, 128, 3)
                           .stride(2)
                           .padding(1)
                           .with_bias(false)
                           .transposed(true)),
        nn::BatchNorm(128),
        nn::Functional(torch::relu),
        // Layer 3
        nn::Conv2d(nn::Conv2dOptions(128, 64, 4)
                           .stride(2)
                           .padding(1)
                           .with_bias(false)
                           .transposed(true)),
        nn::BatchNorm(64),
        nn::Functional(torch::relu),
        // Layer 4
        nn::Conv2d(nn::Conv2dOptions(64, 1, 4)
                           .stride(2)
                           .padding(1)
                           .with_bias(false)
                           .transposed(true)),
        nn::Functional(torch::tanh));

nn::Sequential discriminator(
        // Layer 1
        nn::Conv2d(
                nn::Conv2dOptions(1, 64, 4).stride(2).padding(1).with_bias(false)),
        nn::Functional(torch::leaky_relu, 0.2),
        // Layer 2
        nn::Conv2d(
                nn::Conv2dOptions(64, 128, 4).stride(2).padding(1).with_bias(false)),
        nn::BatchNorm(128),
        nn::Functional(torch::leaky_relu, 0.2),
        // Layer 3
        nn::Conv2d(
                nn::Conv2dOptions(128, 256, 4).stride(2).padding(1).with_bias(false)),
        nn::BatchNorm(256),
        nn::Functional(torch::leaky_relu, 0.2),
        // Layer 4
        nn::Conv2d(
                nn::Conv2dOptions(256, 1, 3).stride(1).padding(0).with_bias(false)),
        nn::Functional(torch::sigmoid));


int main(int, char *[]) {

    //load mnist files
    auto dataset = torch::data::datasets::MNIST("./mnist")
            .map(torch::data::transforms::Normalize<>(0.5, 0.5))
            .map(torch::data::transforms::Stack<>());

    // data loader
    auto data_loader = torch::data::make_data_loader(
            std::move(dataset),
            torch::data::DataLoaderOptions().batch_size(64).workers(2));

    for (torch::data::Example<> &batch : *data_loader) {
        std::cout << "Batch size: " << batch.data.size(0) << " | Labels: ";
        for (int64_t i = 0; i < batch.data.size(0); ++i) {
            std::cout << batch.target[i].item<int64_t>() << " ";
        }
        std::cout << std::endl;
    }

    torch::optim::Adam generator_optimizer(
            generator->parameters(), torch::optim::AdamOptions(2e-4).beta1(0.5));
    torch::optim::Adam discriminator_optimizer(
            discriminator->parameters(), torch::optim::AdamOptions(5e-4).beta1(0.5));


    const int64_t kNumberOfEpochs = 10;

    int64_t batches_per_epoch = 0;

    for (int64_t epoch = 1; epoch <= kNumberOfEpochs; ++epoch) {
        int64_t batch_index = 0;
        for (torch::data::Example<> &batch : *data_loader) {
            // Train discriminator with real images.
            discriminator->zero_grad();
            torch::Tensor real_images = batch.data;
            torch::Tensor real_labels = torch::empty(batch.data.size(0)).uniform_(0.8, 1.0);
            torch::Tensor real_output = discriminator->forward(real_images);
            torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, real_labels);
            d_loss_real.backward();

            // Train discriminator with fake images.
            torch::Tensor noise = torch::randn({batch.data.size(0), kNoiseSize, 1, 1});
            torch::Tensor fake_images = generator->forward(noise);
            torch::Tensor fake_labels = torch::zeros(batch.data.size(0));
            torch::Tensor fake_output = discriminator->forward(fake_images.detach());
            torch::Tensor d_loss_fake = torch::binary_cross_entropy(fake_output, fake_labels);
            d_loss_fake.backward();

            torch::Tensor d_loss = d_loss_real + d_loss_fake;
            discriminator_optimizer.step();

            // Train generator.
            generator->zero_grad();
            fake_labels.fill_(1);
            fake_output = discriminator->forward(fake_images);
            torch::Tensor g_loss = torch::binary_cross_entropy(fake_output, fake_labels);
            g_loss.backward();
            generator_optimizer.step();

            std::printf(
                    "\r[%2lld/%2lld %3lld/%3lld] D_loss: %.4f | G_loss: %.4f",
                    epoch,
                    kNumberOfEpochs,
                    ++batch_index,
                    batches_per_epoch,
                    d_loss.item<float>(),
                    g_loss.item<float>());

            batches_per_epoch = batches_per_epoch < batch_index ? batch_index : batches_per_epoch;
        }
    }

    return 0;
}